// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_FlyTarget.generated.h"

UCLASS()
class OUTPOST_ARCHITECT_API UTask_FlyTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	bool HasReached(const FVector& CurrLoc, const FVector& TargetLoc) const;

public:
	UTask_FlyTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float AcceptRangeRadius = 50.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float FlyHeight = 300.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float HeightSpeed = 50.f;
};
