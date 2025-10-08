// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OaPlayerController.generated.h"

/**
 * 
 */

class UInputAction;
class UBuildManager;
class AOABuildingBase;

UCLASS()
class OUTPOST_ARCHITECT_API AOaPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AOaPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Build System")
	void BuildModeOn(TSubclassOf<AOABuildingBase> BuildingClass);
	UFUNCTION(BlueprintCallable, Category = "Build System")
	void BuildModeOff();
	UFUNCTION(BlueprintCallable, Category = "Build System")
	void TryPlaceBuild();
	UFUNCTION(BlueprintCallable, Category = "Build System")
	void TryDestroyBuilding();
	UFUNCTION(BlueprintPure, Category = "Build System")
	bool InBuildMode() const;

protected:
	UBuildManager* BuildMan;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PlaceBuildingAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CancelBuildingAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DestroyAction;

};
