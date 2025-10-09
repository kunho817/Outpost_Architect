// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Service_EvaluateTarget.generated.h"

/**
 * 
 */
UCLASS()
class OUTPOST_ARCHITECT_API UService_EvaluateTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UService_EvaluateTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	AActor* FindCoreTarget(UWorld* World) const;
	AActor* FindPlayerTarget(UWorld* World) const;
	AActor* EvaluateCurrTarget(class AOAEnemyBase* Enemy, AActor* Core, AActor* Player) const;
	bool IsPlayerInDetectRange(class AOAEnemyBase* Enemy, AActor* Player) const;
};
