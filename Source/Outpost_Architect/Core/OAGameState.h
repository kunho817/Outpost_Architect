// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OAGameState.generated.h"

/**
 * Game State Class
 * Manages game statistics and global resources
 */
UCLASS()
class OUTPOST_ARCHITECT_API AOAGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AOAGameState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 TotalEnemyKill = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 TotalBuildingConstruct = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float GameTime = 0.f;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddEnemyKill() { TotalEnemyKill++; }

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddBuildingCount() { TotalBuildingConstruct++; }

	UPROPERTY(BlueprintReadWrite, Category = "Resource")
	float TotalPower = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Resource")
	float MaxPower = 0.f;

	UFUNCTION(BlueprintPure, Category = "Resource")
	float GetPowerPercent() const { return MaxPower > 0.f ? (TotalPower / MaxPower) : 0.f; }

	UFUNCTION(BlueprintCallable, Category = "Resource")
	void AddMaxPower(float AdditonalPower);

protected:
	virtual void Tick(float DeltaTime) override;
};
