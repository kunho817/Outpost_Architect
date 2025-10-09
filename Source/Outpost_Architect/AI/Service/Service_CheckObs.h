// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Service_CheckObs.generated.h"

/**
 * 
 */
UCLASS()
class OUTPOST_ARCHITECT_API UService_CheckObs : public UBTService
{
	GENERATED_BODY()
	
public:
	UService_CheckObs();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Detection")
	float CheckDist = 500.0f;
	UPROPERTY(EditAnywhere, Category = "Detection")
	bool OnlyCheckCloserThanTarget = true;
};
