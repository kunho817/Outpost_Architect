// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_MoveToTarget.generated.h"

UCLASS()
class OUTPOST_ARCHITECT_API UTask_MoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	bool HasReached(const FVector& CurrLoc, const FVector& TargetLoc) const;

public:
	UTask_MoveToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float AcceptRangeRadius = 50.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float SpeedMult = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool isFly = false;
};
