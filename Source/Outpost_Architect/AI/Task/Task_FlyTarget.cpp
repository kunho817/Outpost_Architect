// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Task/Task_FlyTarget.h"
#include "AIController.h"
#include "Characters/OAEnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UTask_FlyTarget::UTask_FlyTarget()
{
	NodeName = "Fly to Target";
	bNotifyTick = true;
}

EBTNodeResult::Type UTask_FlyTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	AOAEnemyBase* Enemy = Cast<AOAEnemyBase>(AICon->GetPawn());
	if (!Enemy || Enemy->MoveType != EEnemyMoveType::Fly) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UTask_FlyTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	FVector HDir = FVector(TargetLoc.X - CurrLoc.X, 0.f, 0.f);
	HDir.Normalize();

	float TargetH = FlyHeight;
	float CurrH = CurrLoc.Z;
	float HDiff = TargetH - CurrH;

	FVector VAdj = FVector(0.f, 0.f, FMath::Clamp(HDiff, -HeightSpeed * DeltaSeconds, HeightSpeed));

	FVector MoveDir = HDir + VAdj;
	MoveDir.Y = 0.f;

	if (!MoveDir.IsNearlyZero()) {
		Enemy->AddMovementInput(MoveDir.GetSafeNormal(), 1.0f);
		Enemy->UpdateSpriteDir(HDir.X);
	}
}

bool UTask_FlyTarget::HasReached(const FVector& CurrLoc, const FVector& TargetLoc) const
{
	FVector CurrPos = FVector(CurrLoc.X, 0.f, CurrLoc.Z);
	FVector TargetPos = FVector(TargetLoc.X, 0.f, TargetLoc.Z);
	float Dist = FVector::Dist(CurrPos, TargetPos);

	return Dist <= AcceptRangeRadius;
}