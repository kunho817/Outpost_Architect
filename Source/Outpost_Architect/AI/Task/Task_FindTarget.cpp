// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/Task_FindTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/OAEnemyBase.h"

UTask_FindTarget::UTask_FindTarget()
{
	NodeName = "Find Target";
}

EBTNodeResult::Type UTask_FindTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMem)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	AOAEnemyBase* Enemy = Cast<AOAEnemyBase>(AICon->GetPawn());
	if(!Enemy) return EBTNodeResult::Failed;

	AActor* Target = Enemy->FindTarget();

	if (Target) {
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetActorKey.SelectedKeyName, Target);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
