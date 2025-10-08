// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OAEnum.h"
#include "OAStruct.h"
#include "Buildable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBuildable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OUTPOST_ARCHITECT_API IBuildable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Building")
	EBuildingType GetBType() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Building")
	FBuildCost GetBCost() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Building")
	bool CanBuild(const FVector& Loc) const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Building")
	void ConstructBuilding(const FVector& Loc);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Building")
	void DestoryBuliding();
};
