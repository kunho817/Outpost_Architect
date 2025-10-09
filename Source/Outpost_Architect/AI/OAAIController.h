// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "OAAIController.generated.h"

class UBehaviorTree;
class UBlackboardComponent;

UCLASS()
class OUTPOST_ARCHITECT_API AOAAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AOAAIController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BT;

	static const FName CurrentActorKey;
	static const FName CoreTargetKey;
	static const FName PlayerTargetKey;
	static const FName ObstacleTargetKey;
	static const FName SelfActorKey;
	static const FName AtkRangeKey;
	static const FName DetectRangeKey;
	static const FName EnemyTypeKey;

protected:
	virtual void OnPossess(APawn* IPawn) override;
	virtual void OnUnPossess() override;
};
