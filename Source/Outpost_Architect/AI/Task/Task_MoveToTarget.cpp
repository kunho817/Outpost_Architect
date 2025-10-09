// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/Task_MoveToTarget.h"
#include "AI/OAAIController.h"
#include "AIController.h"
#include "Characters/OAEnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UTask_MoveToTarget::UTask_MoveToTarget()
{
	NodeName = "Move To Target";
	bNotifyTick = true;
}

EBTNodeResult::Type UTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	AOAEnemyBase* Enemy = Cast<AOAEnemyBase>(AICon->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;
	
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	AActor* TargetA = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetA) return EBTNodeResult::Failed;

	if (HasReached(Enemy->GetActorLocation(), TargetA->GetActorLocation())) return EBTNodeResult::Succeeded;
	if (UCharacterMovementComponent* MoveComp = Enemy->GetCharacterMovement()) MoveComp->MaxWalkSpeed = Enemy->MoveSpeed * SpeedMult;

	return EBTNodeResult::InProgress;
}

void UTask_MoveToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

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

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AActor* TargetA = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetA) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FVector CurrLoc = Enemy->GetActorLocation();
	FVector TargetLoc = TargetA->GetActorLocation();

	if (HasReached(CurrLoc, TargetLoc)) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	FVector Dir = (TargetLoc - CurrLoc).GetSafeNormal();

	if (!isFly) {
		Dir.Y = 0.f;
		Dir.Z = 0.f;
		Dir.Normalize();
	}
	else {
		Dir.Y = 0.f;
		Dir.Normalize();
	}

	Enemy->AddMovementInput(Dir, 1.0f);
	Enemy->UpdateSpriteDir(Dir.X);
}

bool UTask_MoveToTarget::HasReached(const FVector& CurrLoc, const FVector& TargetLoc) const
{
	float Dist;

	if (isFly) {
		FVector CurrPos = FVector(CurrLoc.X, 0.f, CurrLoc.Z);
		FVector TargetPos = FVector(TargetLoc.X, 0.f, TargetLoc.Z);
		Dist = FVector::Dist(CurrPos, TargetPos);
	}
	else {
		Dist = FMath::Abs(CurrLoc.X - TargetLoc.X);
	}

	return Dist <= AcceptRangeRadius;
}