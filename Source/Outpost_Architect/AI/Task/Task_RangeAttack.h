// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_RangeAttack.generated.h"

UCLASS()
class OUTPOST_ARCHITECT_API UTask_RangeAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTask_RangeAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;
};
