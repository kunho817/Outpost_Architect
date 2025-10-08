// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/Task_BackMove.h"
#include "AIController.h"
#include "Characters/OAEnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UTask_BackMove::UTask_BackMove()
{
	NodeName = "Back Moving";
	bNotifyTick = true;
}

EBTNodeResult::Type UTask_BackMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMem)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	AOAEnemyBase* Enemy = Cast<AOAEnemyBase>(AICon->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	AActor* Target = Enemy->GetTarget();
	if (!Target) return EBTNodeResult::Failed;

	FVector EnemyLoc = Enemy->GetActorLocation();
	FVector TargetLoc = Target->GetActorLocation();
	FVector Dir = (EnemyLoc - TargetLoc).GetSafeNormal();

	FVector BackMoveLoc = EnemyLoc + (Dir * Enemy->BackMoveDist);

	if (UCharacterMovementComponent* MoveComp = Enemy->GetCharacterMovement()) MoveComp->MaxWalkSpeed = BackMoveSpeed;

	return EBTNodeResult::InProgress;
}

void UTask_BackMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMem, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMem, DeltaSeconds);

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AOAEnemyBase* Enemy = Cast<AOAEnemyBase>(AICon->GetPawn());
	if (!Enemy) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AActor* Target = Enemy->GetTarget();
	if (!Target) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	float DistTarget = Enemy->GetDistTarget();

	if (DistTarget >= Enemy->BackMoveDist) {
		if (UCharacterMovementComponent* MoveComp = Enemy->GetCharacterMovement()) MoveComp->MaxWalkSpeed = Enemy->MoveSpeed;

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	FVector EnemyLoc = Enemy->GetActorLocation();
	FVector TargetLoc = Target->GetActorLocation();
	FVector Dir = (EnemyLoc - TargetLoc).GetSafeNormal();

	FVector MoveDir = FVector(Dir.X, 0.f, 0.f);

	Enemy->AddMovementInput(MoveDir, 1.0f);
}