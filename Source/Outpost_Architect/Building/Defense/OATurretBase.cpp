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

	DetectRange = CreateDefaultSubobject<USphereComponent>(TEXT("DetectRange"));
	DetectRange->SetupAttachment(RootComponent);
	DetectRange->SetSphereRadius(500.0f);
	DetectRange->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	DetectRange->OnComponentBeginOverlap.AddDynamic(this, &AOATurretBase::OnDetectBeginOverlap);
	DetectRange->OnComponentEndOverlap.AddDynamic(this, &AOATurretBase::OnDetectEndOverlap);

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
	RotateSpeed = 180.0f;

	AtkDmg = 10.0f;
	AtkCool = 1.0f;

	CurrTarget = nullptr;
	PrevAtkTime = -999.0f;

}

void AOATurretBase::BeginPlay()
{
	Super::BeginPlay();

	DetectRange->SetSphereRadius(DectectRadius);

	GetWorldTimerManager().SetTimer(SearchTimerHandler, this, &AOATurretBase::SearchTarget, 0.5f, true);
}

void AOATurretBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsOperation() || !HasGenPower()) return;
	if (CurrTarget) {
		RotateToTarget(DeltaTime);
		if (IsAimToTarget() && CanAtk()) DoAtk();
	}
}

void AOATurretBase::SearchTarget()
{
	if (!IsOperation() || !HasGenPower()) return;

	if (CurrTarget && !IsValidTarget(CurrTarget)) {
		CurrTarget = nullptr;
		OnTargetLost();
	}
	if (!CurrTarget) SelectTarget();
}

void AOATurretBase::SelectTarget()
{
	if (DetectEnemy.Num() == 0) return;

	TArray<AActor*> ValidTarget;
	for (AActor* Enemy : DetectEnemy) {
		if (IsValidTarget(Enemy)) ValidTarget.Add(Enemy);
	}

	if (ValidTarget.Num() == 0) return;

	AActor* BestTarget = nullptr;
	switch (Prio)
	{
	case ETargetPriority::Closest: {
		float Closest = FLT_MAX;
		for (AActor* Target : ValidTarget) {
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
		for (AActor* Target : ValidTarget) {
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
		for (AActor* Target : ValidTarget) {
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
		for (AActor* Target : ValidTarget) {
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
	if (!ProjClass || !CurrTarget) return;

	FVector SpawnLoc = ShotPoint->GetComponentLocation();
	FVector Dir = (CurrTarget->GetActorLocation() - SpawnLoc).GetSafeNormal();

	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = this;
	SpawnParam.Instigator = GetInstigator();

	AOAProjectile* Proj = GetWorld()->SpawnActor<AOAProjectile>(ProjClass, SpawnLoc, Dir.Rotation(), SpawnParam);

	if (Proj) Proj->InitProjectile(AtkDmg, this, Dir);
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

void AOATurretBase::OnDetectBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(AOAEnemyBase::StaticClass())) DetectEnemy.AddUnique(OtherActor);
}

void AOATurretBase::OnDetectEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	DetectEnemy.Remove(OtherActor);

	if (CurrTarget == OtherActor) {
		CurrTarget = nullptr;
		OnTargetLost();
	}
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

