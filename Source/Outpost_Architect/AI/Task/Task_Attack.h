// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_Attack.generated.h"

UCLASS()
class OUTPOST_ARCHITECT_API UTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMem) override;
};
