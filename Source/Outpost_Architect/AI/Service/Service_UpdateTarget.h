// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Service_UpdateTarget.generated.h"

UCLASS()
class OUTPOST_ARCHITECT_API UService_UpdateTarget : public UBTService
{
	GENERATED_BODY()

public:
	UService_UpdateTarget();

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMem, float DeltaSeconds) override;
};
