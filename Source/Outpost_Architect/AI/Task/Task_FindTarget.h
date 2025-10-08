// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_FindTarget.generated.h"

UCLASS()
class OUTPOST_ARCHITECT_API UTask_FindTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTask_FindTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMem) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;
};
