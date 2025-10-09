// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_AttackObs.generated.h"

/**
 * 
 */
UCLASS()
class OUTPOST_ARCHITECT_API UTask_AttackObs : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTask_AttackObs();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRange = 150.f;
};
