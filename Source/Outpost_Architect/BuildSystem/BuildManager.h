// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OAEnum.h"
#include "OAStruct.h"
#include "BuildSystem/OAGridSystem.h"
#include "BuildManager.generated.h"

class AOABuildingBase;
class UInputAction;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildModeChange, bool, IsActive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlacementChange, bool, IsCanPlace, FVector, Loc);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OUTPOST_ARCHITECT_API UBuildManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY()
	UOAGridSystem* Grid;
	UPROPERTY()
	APlayerController* OwnerCon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Build")
	bool BuildModeActive;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Build")
	TSubclassOf<AOABuildingBase> SelectBuildingClass;

	UPROPERTY()
	AOABuildingBase* Ghost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build|Ghost")
	UMaterialInterface* ValidMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build|Ghost")
	UMaterialInterface* InvalidMat;

	FVector CurrPlaceLoc;
	FGridCoord CurrCoord;
	bool CanPlace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build|Settings")
	float MaxDist;

public:
	UFUNCTION(BlueprintCallable, Category = "Build")
	void SetPlayerCon(APlayerController* PC) { OwnerCon = PC; }

	UFUNCTION(BlueprintCallable, Category = "Build")
	void BuildModeOn(TSubclassOf<AOABuildingBase> BuildingClass);
	UFUNCTION(BlueprintCallable, Category = "Build")
	void BuildModeOff();
	UFUNCTION(BlueprintPure, Category = "Build")
	bool IsInBuildMode() const { return BuildModeActive; }

	UFUNCTION(BlueprintCallable, Category = "Build")
	void UpdateGhostPos();
	UFUNCTION(BlueprintCallable, Category = "Build")
	bool TryPlaceBuilding();
	UFUNCTION(BlueprintCallable, Category = "Build")
	bool CanPlaceBuilding();

	UFUNCTION(BlueprintCallable, Category = "Build")
	void DestroyBuildingAtLocation();

	UFUNCTION(BlueprintCallable, Category = "Build")
	bool HasEnoughResource(FBuildCost BCost);
	UFUNCTION(BlueprintCallable, Category = "Build")
	void ConsumeResource(FBuildCost BCost);

	UPROPERTY(BlueprintAssignable, Category = "Build Events")
	FOnBuildModeChange OnBuildModeChange;
	UPROPERTY(BlueprintAssignable, Category = "Build Events")
	FOnPlacementChange OnPlacementChange;

protected:
	void CreateGhost();
	void DestroyGhost();
	void UpdateGhostMat();

	FVector GetMousePos();

};
