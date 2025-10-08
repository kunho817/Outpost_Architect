// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/OABuildingBase.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"

// Sets default values
AOABuildingBase::AOABuildingBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	ColComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	ColComp->SetCollisionProfileName(TEXT("Building"));
	RootComponent = ColComp;

	SpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComp->SetupAttachment(RootComponent);

	BType = EBuildingType::Core;
	BState = EBuildingState::Construct;

	MaxHealth = 100.0f;
	CurrHealth = MaxHealth;

	ConstTime = 20.0f;
	ConstProg = 0.0f;

	PowerCons = 0.0f;
	PowerProd = 0.0f;
	GenPower = true;
}

// Called when the game starts or when spawned
void AOABuildingBase::BeginPlay()
{
	Super::BeginPlay();
	
	CurrHealth = MaxHealth;
}

// Called every frame
void AOABuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BState == EBuildingState::Construct) UpdateConstruct(DeltaTime);
}

void AOABuildingBase::TakeDamage_Implementation(const FDamage& DInfo)
{
	if (BState == EBuildingState::Destroyed) return;

	CurrHealth -= DInfo.Damage;

	if (CurrHealth <= 0) {
		CurrHealth = 0;
		Die_Implementation();
	}
}

void AOABuildingBase::Die_Implementation()
{
	SetBState(EBuildingState::Destroyed);
	OnBuildingDestroy();

	ColComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle DTHandler;
	GetWorldTimerManager().SetTimer(DTHandler, [this]() { Destroy(); }, 2.0f, false);
}

bool AOABuildingBase::CanBuild_Implementation(const FVector& Loc) const
{
	return true;
}

void AOABuildingBase::ConstructBuilding_Implementation(const FVector& Loc)
{
	SetActorLocation(Loc);
	StartConstruct();
}

void AOABuildingBase::DestoryBuliding_Implementation()
{
	Destroy();
}

void AOABuildingBase::SetBState(EBuildingState NState)
{
	if (BState == NState) return;

	BState = NState;

	switch (BState)
	{
	case EBuildingState::Enabled:
		SetActorTickEnabled(true);
		break;
	case EBuildingState::Disabled:
	case EBuildingState::Destroyed:
		SetActorTickEnabled(false);
		break;
	default:
		break;
	}
}

void AOABuildingBase::StartConstruct()
{
	BState = EBuildingState::Construct;
	ConstProg = 0.0f;

	CurrHealth = 0.0f;
}

void AOABuildingBase::UpdateConstruct(float DeltaTime)
{
	if (ConstTime <= 0.0f) {
		CompleteConstruct();
		return;
	}

	ConstProg += DeltaTime / ConstTime;
	CurrHealth = MaxHealth * ConstProg;

	if (ConstProg >= 1.0f) CompleteConstruct();
}

void AOABuildingBase::CompleteConstruct()
{
	ConstProg = 1.0f;
	SetBState(EBuildingState::Enabled);
	CurrHealth = MaxHealth;

	OnBuildingConstruct();
}

void AOABuildingBase::SetPowerState(bool Powered)
{
	if (GenPower == Powered) return;

	GenPower = Powered;

	if (BState == EBuildingState::Enabled || BState == EBuildingState::Disabled) SetBState(Powered ? EBuildingState::Enabled : EBuildingState::Disabled);

	OnPowerStateChange(Powered);
}

void AOABuildingBase::OnBuildingConstruct_Implementation()
{
}

void AOABuildingBase::OnBuildingDestroy_Implementation()
{
}

void AOABuildingBase::OnPowerStateChange_Implementation(bool Powered)
{
}
