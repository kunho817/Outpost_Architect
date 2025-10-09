// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Task/Task_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/OAAIController.h"
#include "Characters/OAEnemyBase.h"

UTask_Attack::UTask_Attack()
{
	NodeName = "Attack Target";
}

EBTNodeResult::Type UTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	AOAEnemyBase* Enemy = Cast<AOAEnemyBase>(AICon->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(AOAAIController::CurrentActorKey));
	if(!Target) return EBTNodeResult::Failed;

	if (Enemy->CanAtk()) {
		Enemy->DoAtk(Target);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}