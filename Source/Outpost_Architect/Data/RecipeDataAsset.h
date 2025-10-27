// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OAStruct.h"
#include "RecipeDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class OUTPOST_ARCHITECT_API URecipeDataAsset : public UDataAsset
{
	GENERATED_BODY()
	

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
    FName RecipeID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
    FText RecipeName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
    FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
    TArray<FRecipeIngredient> Input;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
    FRecipeIngredient Output;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
    float CraftTime = 5.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
    bool ReqFuel = false;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "ReqFuel"), Category = "Recipe")
    float FuelPerCraft = 1.0f;

    UFUNCTION(BlueprintPure, Category = "Recipe")
    FCraftRecipe ToRecipeData() const;

    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
