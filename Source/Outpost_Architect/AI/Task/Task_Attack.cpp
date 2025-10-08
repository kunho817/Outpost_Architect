// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Task/Task_Attack.h"
#include "AIController.h"
#include "Characters/OAEnemyBase.h"

UTask_Attack::UTask_Attack()
{
	NodeName = "Attack Target";
}

EBTNodeResult::Type UTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMem)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	AOAEnemyBase* Enemy = Cast<AOAEnemyBase>(AICon->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	if (Enemy->CanAtk()) {
		Enemy->DoAtk();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}