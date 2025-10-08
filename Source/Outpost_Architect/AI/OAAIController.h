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

	static const FName TargetActorKey;
	static const FName SelfActorKey;
	static const FName AtkRangeKey;
	static const FName DetectRangeKey;
	static const FName EnemyTypeKey;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTargetActor(AActor* Target);
	UFUNCTION(BlueprintPure, Category = "AI")
	AActor* GetTargetActor() const;

protected:
	virtual void OnPossess(APawn* Pawn) override;
	virtual void OnUnPossess() override;
};
