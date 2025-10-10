// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/OAProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PaperSpriteComponent.h"
#include "Interfaces/OnDamage.h"
#include "Characters/OA2DCharacterBase.h"
#include "Building/OABuildingBase.h"
#include "OAStruct.h"
#include "TimerManager.h"

// Sets default values
AOAProjectile::AOAProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ColComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	ColComp->InitSphereRadius(15.0f);
	ColComp->SetCollisionProfileName(TEXT("Projectile"));
	ColComp->OnComponentHit.AddDynamic(this, &AOAProjectile::OnHit);
	RootComponent = ColComp;

	SpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComp->SetupAttachment(RootComponent);

	ProjMove = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjMove->UpdatedComponent = ColComp;
	ProjMove->InitialSpeed = 600.0f;
	ProjMove->MaxSpeed = 600.0f;
	ProjMove->bRotationFollowsVelocity = true;
	ProjMove->bShouldBounce = false;
	ProjMove->ProjectileGravityScale = 0.0f;

	Dmg = 10.0f;
	ProjSpeed = 600.0f;
	MaxLifeTime = 10.0f;
	DestroyOnHit = true;
	ProjInstigator = nullptr;

}

void AOAProjectile::InitProjectile(float NDmg, AActor* IInstigator, FVector Dir)
{
	Dmg = NDmg;
	ProjInstigator = IInstigator;

	ProjMove->Velocity = Dir.GetSafeNormal() * ProjSpeed;

	if (AOABuildingBase* B = Cast<AOABuildingBase>(IInstigator)) OwnerTeam = B->GetTeam();
	else if (AOA2DCharacterBase* C = Cast<AOA2DCharacterBase>(IInstigator)) OwnerTeam = C->GetTeam();
}

// Called when the game starts or when spawned
void AOAProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(LifeTimerHandler, this, &AOAProjectile::EndLifeTime, MaxLifeTime, false);
}

void AOAProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherA, UPrimitiveComponent* OtherComp, FVector NormalImpusle, const FHitResult& hit)
{
	if (OtherA == this || OtherA == ProjInstigator) return;
	if (IsFriendly(OtherA)) return;

	ApplyDamage(OtherA);

	if (DestroyOnHit) {
		OnProjDestroy();
		Destroy();
	}
}

void AOAProjectile::ApplyDamage(AActor* Target)
{
	if (!Target) return;

	UE_LOG(LogTemp, Log, TEXT("%s Damaged %.1f by Projectile"), *Target->GetName(), Dmg);

	if (Target->GetClass()->ImplementsInterface(UOnDamage::StaticClass())) {
		FDamage DInfo;
		DInfo.Damage = Dmg;
		DInfo.Instigator = ProjInstigator;
		IOnDamage::Execute_TakeDamage(Target, DInfo);
	}
}

void AOAProjectile::OnProjDestroy_Implementation()
{
}

void AOAProjectile::EndLifeTime()
{
	OnProjDestroy();
	Destroy();
}

bool AOAProjectile::IsFriendly(AActor* Target) const
{
	if (!Target) return false;
	if (OwnerTeam == ETeam::Neutral) return false;

	ETeam TargetTeam = ETeam::Neutral;

	if (AOABuildingBase* B = Cast<AOABuildingBase>(Target)) TargetTeam = B->GetTeam();
	else if (AOA2DCharacterBase* C = Cast<AOA2DCharacterBase>(Target)) TargetTeam = C->GetTeam();

	return TargetTeam == OwnerTeam;
}
