// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/OAEnemyBase.h"
#include "AI/OAAIController.h"
#include "Core/OAGameMode.h"
#include "Core/OAWaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AOAEnemyBase::AOAEnemyBase()
{
	AIControllerClass = AOAAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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

	FindTarget();
}

void AOAEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector V = GetCharacterMovement()->Velocity;
	if (!V.IsNearlyZero()) UpdateSpriteDir(V.X);
}

void AOAEnemyBase::SetTarget(AActor* NewT)
{
	TargetA = NewT;

	if (AOAAIController* AICon = Cast<AOAAIController>(GetController())) AICon->SetTargetActor(NewT);
}

float AOAEnemyBase::GetDistTarget() const
{
	if (!TargetA) return -1.f;

	return FVector::Dist(GetActorLocation(), TargetA->GetActorLocation());
}

AActor* AOAEnemyBase::FindTarget()
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);

	if (Player) {
		SetTarget(Player);
		return Player;
	}

	return nullptr;
}

void AOAEnemyBase::DoAtk()
{
	if (!CanAtk() || !TargetA) return;

	PrevAtkTime = GetWorld()->GetTimeSeconds();

	if (IOnDamage* DamageSys = Cast<IOnDamage>(TargetA)) {
		FDamage DInfo;
		DInfo.Damage = AtkDmg;
		DInfo.Instigator = this;

		IOnDamage::Execute_TakeDamage(TargetA, DInfo);
	}
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