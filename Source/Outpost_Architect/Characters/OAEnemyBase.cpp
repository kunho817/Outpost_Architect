// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/OAEnemyBase.h"
#include "AI/OAAIController.h"
#include "Core/OAGameMode.h"
#include "Core/OAWaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interfaces/OnDamage.h"

AOAEnemyBase::AOAEnemyBase()
{
	AIControllerClass = AOAAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	if (UCapsuleComponent* Col = GetCapsuleComponent()) Col->SetCollisionProfileName(TEXT("Enemy"));
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement()) MoveComp->bUseRVOAvoidance = false;

	SetTeam(ETeam::Enemy);
}

void AOAEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement()) {
		if (MoveType == EEnemyMoveType::Fly) {
			MoveComp->SetMovementMode(MOVE_Flying);
			MoveComp->GravityScale = 0.f;
		}
		else {
			MoveComp->SetMovementMode(MOVE_Walking);
			MoveComp->GravityScale = 1.f;
		}

		MoveComp->MaxWalkSpeed = MoveSpeed;
		MoveComp->MaxFlySpeed = MoveSpeed;
	}

	if (AOAGameMode* GM = Cast<AOAGameMode>(UGameplayStatics::GetGameMode(this))) {
		WaveMan = GM->GetWaveMan();

		if (WaveMan) WaveMan->RegisterEnemy(this);
	}
}

void AOAEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector V = GetCharacterMovement()->Velocity;
	if (!V.IsNearlyZero()) UpdateSpriteDir(V.X);
}

void AOAEnemyBase::DoAtk(AActor* Target)
{
	if (!CanAtk() || !Target) return;

	PrevAtkTime = GetWorld()->GetTimeSeconds();

	if (IOnDamage* DamageSys = Cast<IOnDamage>(Target)) {
		FDamage DInfo;
		DInfo.Damage = AtkDmg;
		DInfo.Instigator = this;

		IOnDamage::Execute_TakeDamage(Target, DInfo);
	}
}

void AOAEnemyBase::DoAtk()
{
	if (!CanAtk()) return;

	PrevAtkTime = GetWorld()->GetTimeSeconds();
}

bool AOAEnemyBase::CanAtk() const
{
	float CurrTime = GetWorld()->GetTimeSeconds();
	return (CurrTime - PrevAtkTime) >= AtkCool;
}

void AOAEnemyBase::Die_Implementation()
{
	if (WaveMan) WaveMan->UnregisterEnemy(this);

	Super::Die_Implementation();
}

void AOAEnemyBase::DeathFinish()
{
	Super::DeathFinish();
}