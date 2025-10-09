// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Defense/OATurretBase.h"
#include "Components/SphereComponent.h"
#include "Combat/OAProjectile.h"
#include "Interfaces/OnDamage.h"
#include "Characters/OAEnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AOATurretBase::AOATurretBase()
{
	PrimaryActorTick.bCanEverTick = true;

	TurretHead = CreateDefaultSubobject<USceneComponent>(TEXT("TurretHead"));
	TurretHead->SetupAttachment(RootComponent);

	ShotPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ShootPoint"));
	ShotPoint->SetupAttachment(TurretHead);
	ShotPoint->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));

	BType = EBuildingType::Defense;

	AtkType = ETurretAttackType::Projectile;
	Prio = ETargetPriority::Closest;

	DectectRadius = 500.0f;
	AtkRange = 450.0f;
	RotateSpeed = 1.0f;

	AtkDmg = 10.0f;
	AtkCool = 1.0f;

	CurrTarget = nullptr;
	PrevAtkTime = -999.0f;
	GenPower = true;
}

void AOATurretBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(SearchTimerHandler, this, &AOATurretBase::SearchTarget, 0.5f, true);
}

void AOATurretBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsOperation() || !HasGenPower() || GetGhostState()) return;
	if (CurrTarget) {
		RotateToTarget(DeltaTime);
		if (IsAimToTarget() && CanAtk()) DoAtk();
	}
}

void AOATurretBase::SearchTarget()
{
	if (!IsOperation() || !HasGenPower()) return;

	if (CurrTarget && !IsValidTarget(CurrTarget)) {
		UE_LOG(LogTemp, Log, TEXT("  Turret %s: Current target invalid, clearing"), *GetName());
		CurrTarget = nullptr;
		OnTargetLost();
	}
	if (!CurrTarget) {
		TArray<FOverlapResult> OverlapRes;
		FCollisionQueryParams QParam;
		QParam.AddIgnoredActor(this);

		FVector Loc = GetActorLocation();
		FCollisionShape Sphere = FCollisionShape::MakeSphere(DectectRadius);

		bool IsHit = GetWorld()->OverlapMultiByChannel(OverlapRes, Loc, FQuat::Identity, ECC_Pawn, Sphere, QParam);
		if (IsHit) {
			TArray<AActor*> DetectedEnemy;
			for (const FOverlapResult& Res : OverlapRes) {
				if (Res.GetActor() && Res.GetActor()->IsA(AOAEnemyBase::StaticClass())) DetectedEnemy.Add(Res.GetActor());
			}

			if (DetectedEnemy.Num() > 0) SelectTarget(DetectedEnemy);
		}
	}
}

void AOATurretBase::SelectTarget(const TArray<AActor*>& EnemyArr)
{
	TArray<AActor*> ValidT;
	for (AActor* E : EnemyArr) {
		if (IsValidTarget(E)) ValidT.Add(E);
	}

	if (ValidT.Num() == 0) return;

	AActor* BestTarget = nullptr;
	switch (Prio)
	{
	case ETargetPriority::Closest: {
		float Closest = FLT_MAX;
		for (AActor* Target : ValidT) {
			float Dist = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
			if (Dist < Closest) {
				Closest = Dist;
				BestTarget = Target;
			}
		}
		break;
	}

	case ETargetPriority::Farthest: {
		float Farthest = FLT_MIN;
		for (AActor* Target : ValidT) {
			float Dist = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
			if (Dist > Farthest) {
				Farthest = Dist;
				BestTarget = Target;
			}
		}
		break;
	}

	case ETargetPriority::LowHP: {
		float Low = FLT_MAX;
		for (AActor* Target : ValidT) {
			if (Target->GetClass()->ImplementsInterface(UOnDamage::StaticClass())) {
				float HP = IOnDamage::Execute_GetCurrHealth(Target);
				if (HP < Low) {
					Low = HP;
					BestTarget = Target;
				}
			}
		}
		break;
	}

	case ETargetPriority::HighHP: {
		float High = FLT_MIN;
		for (AActor* Target : ValidT) {
			if (Target->GetClass()->ImplementsInterface(UOnDamage::StaticClass())) {
				float HP = IOnDamage::Execute_GetCurrHealth(Target);
				if (HP > High) {
					High = HP;
					BestTarget = Target;
				}
			}
		}
		break;
	}

	}

	if (BestTarget) {
		CurrTarget = BestTarget;
		OnTargetAcquire(BestTarget);
	}
}

bool AOATurretBase::IsValidTarget(AActor* Target)
{
	if (!Target) return false;

	if (Target->GetClass()->ImplementsInterface(UOnDamage::StaticClass()))
		if (!IOnDamage::Execute_Alive(Target)) return false;

	float Dist = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
	if (Dist > AtkRange) return false;

	return true;
}

void AOATurretBase::DoAtk()
{
	if (!CanAtk()) return;

	PrevAtkTime = GetWorld()->GetTimeSeconds();

	switch (AtkType)
	{
	case ETurretAttackType::Projectile:
		ShootProjectile();
		break;
	case ETurretAttackType::Hitscan:
		ShootHitscan();
		break;
	}
}

bool AOATurretBase::CanAtk()
{
	if (!CurrTarget) return false;

	float CurrTime = GetWorld()->GetTimeSeconds();
	return (CurrTime - PrevAtkTime) >= AtkCool;
}

void AOATurretBase::ShootProjectile()
{
	if (!ProjClass || !CurrTarget) {
		UE_LOG(LogTemp, Warning, TEXT("⚠️ Turret %s: Cannot shoot projectile (ProjClass: %s, Target: %s)"),
			*GetName(),
			ProjClass ? TEXT("OK") : TEXT("NULL"),
			CurrTarget ? TEXT("OK") : TEXT("NULL"));
		return;
	}

	FVector SpawnLoc = ShotPoint->GetComponentLocation();
	FVector Dir = (CurrTarget->GetActorLocation() - SpawnLoc).GetSafeNormal();

	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = this;
	SpawnParam.Instigator = GetInstigator();

	AOAProjectile* Proj = GetWorld()->SpawnActor<AOAProjectile>(ProjClass, SpawnLoc, Dir.Rotation(), SpawnParam);

	if (Proj) {
		Proj->InitProjectile(AtkDmg, this, Dir);
		UE_LOG(LogTemp, Log, TEXT("  Turret %s: Projectile spawned"), *GetName());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("❌ Turret %s: Failed to spawn projectile!"), *GetName());
	}
}

void AOATurretBase::ShootHitscan()
{
	if (!CurrTarget) return;

	FVector Start = ShotPoint->GetComponentLocation();
	FVector End = CurrTarget->GetActorLocation();

	FHitResult HitRes;
	FCollisionQueryParams QueryParam;
	QueryParam.AddIgnoredActor(this);

	bool Hit = GetWorld()->LineTraceSingleByChannel(HitRes, Start, End, ECC_Visibility, QueryParam);

	if (Hit && HitRes.GetActor()) {
		if (HitRes.GetActor()->GetClass()->ImplementsInterface(UOnDamage::StaticClass())) {
			FDamage DInfo;
			DInfo.Damage = AtkDmg;
			DInfo.Instigator = this;

			IOnDamage::Execute_TakeDamage(HitRes.GetActor(), DInfo);
		}
	}
}

void AOATurretBase::RotateToTarget(float DeltaTime)
{
	if (!CurrTarget) return;

	FVector TargetLoc = CurrTarget->GetActorLocation();
	FVector CurrLoc = TurretHead->GetComponentLocation();

	FVector Dir = (TargetLoc - CurrLoc).GetSafeNormal2D();
	FRotator TargetRot = Dir.Rotation();

	FRotator CurrRot = TurretHead->GetComponentRotation();
	FRotator NewRot = FMath::RInterpTo(CurrRot, TargetRot, DeltaTime, RotateSpeed);

	TurretHead->SetWorldRotation(NewRot);
}

bool AOATurretBase::IsAimToTarget()
{
	if (!CurrTarget) return false;

	FVector TargetLoc = CurrTarget->GetActorLocation();
	FVector CurrLoc = TurretHead->GetComponentLocation();
	FVector Dir = (TargetLoc - CurrLoc).GetSafeNormal2D();

	FVector ForwardV = TurretHead->GetForwardVector();

	float DotProduct = FVector::DotProduct(ForwardV, Dir);

	return DotProduct > FMath::Cos(FMath::DegreesToRadians(5.0f));
}

void AOATurretBase::OnTargetAcquire_Implementation(AActor* Target)
{
}

void AOATurretBase::OnTargetLost_Implementation()
{
}

void AOATurretBase::OnFire_Implementation()
{
}

