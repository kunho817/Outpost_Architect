// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_BackMove.generated.h"

UCLASS()
class OUTPOST_ARCHITECT_API UTask_BackMove : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTask_BackMove();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Task")
	float BackMoveSpeed = 200.f;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
