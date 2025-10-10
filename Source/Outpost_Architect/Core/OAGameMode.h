// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OAStruct.h"
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

	UFUNCTION(BlueprintPure, Category = "Data")
	FItemData GetItemData(FName ItemID) const;
	UFUNCTION(BlueprintPure, Category = "Data")
	UDataTable* GetDataTable() const { return ItemData; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "System")
	UOAWaveManager* WaveMan;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "System")
	UOAGridSystem* Grid;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "System")
	UBuildManager* BuildMan;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UDataTable* ItemData = nullptr;

private:
	UFUNCTION()
	void OnWaveComplete(int32 WaveNum);
};
