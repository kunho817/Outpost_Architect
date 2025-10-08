// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OAGameMode.generated.h"

class UOAWaveManager;
class UBuildManager;
class UOAGridSystem;

UCLASS()
class OUTPOST_ARCHITECT_API AOAGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOAGameMode();

	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void GameOver();

	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void Victory();

	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void OnBaseCoreDestroy();

	UFUNCTION(BlueprintPure, Category = "System")
	UOAWaveManager* GetWaveMan() const { return WaveMan; }
	UFUNCTION(BlueprintPure, Category = "System")
	UOAGridSystem* GetGrid() const { return Grid; }
	UFUNCTION(BlueprintPure, Category = "System")
	UBuildManager* GetBuildMan() const { return BuildMan; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "System")
	UOAWaveManager* WaveMan;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "System")
	UOAGridSystem* Grid;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "System")
	UBuildManager* BuildMan;

private:
	UFUNCTION()
	void OnWaveComplete(int32 WaveNum);
};
