// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/OA2DCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"

AOA2DCharacterBase::AOA2DCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MaxFlySpeed = 300.f;


	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.f, -1.f, 0.f));

	GetSprite()->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
}

// Called when the game starts or when spawned
void AOA2DCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	CurrHealth = MaxHealth;

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	GetCharacterMovement()->JumpZVelocity = JumpH;
}

// Called every frame
void AOA2DCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOA2DCharacterBase::TakeDamage_Implementation(const FDamage& DInfo)
{
	if (!Alive_Implementation()) return;

	CurrHealth -= DInfo.Damage;
	CurrHealth = FMath::Max(0.f, CurrHealth);

	OnDamageReceive(DInfo.Damage);

	if (CurrHealth <= 0.f) Die_Implementation();
}

float AOA2DCharacterBase::GetCurrHealth_Implementation() const
{
	return CurrHealth;
}

float AOA2DCharacterBase::GetMaxHealth_Implementation() const
{
	return MaxHealth;
}

bool AOA2DCharacterBase::Alive_Implementation() const
{
	return CurrHealth > 0.f;
}

void AOA2DCharacterBase::Die_Implementation()
{
	//Death();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->DisableMovement();

	DeathFinish();
}

void AOA2DCharacterBase::DeathFinish()
{
	Destroy();
	//SetLifeSpan(2.0f);
}

void AOA2DCharacterBase::Heal(float Heal)
{
	if (!Alive_Implementation()) return;
	CurrHealth = FMath::Min(CurrHealth + Heal, MaxHealth);
}

void AOA2DCharacterBase::UpdateSpriteDir(float Dir)
{
	if (Dir != 0.f) {
		LastDir = Dir;

		FVector Scale = GetSprite()->GetRelativeScale3D();
		Scale.X = FMath::Sign(Dir) * FMath::Abs(Scale.X);
		GetSprite()->SetRelativeScale3D(Scale);
	}
}