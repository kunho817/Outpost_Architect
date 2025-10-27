// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/RecipeDataAsset.h"
#include "RecipeDatabase.generated.h"

/**
 * 
 */
UCLASS()
class OUTPOST_ARCHITECT_API URecipeDatabase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Database")
	TArray<URecipeDataAsset*> AllRecipe;

protected:
	UPROPERTY(Transient)
	TMap<FName, URecipeDataAsset*> RecipeMap;
	UPROPERTY(Transient)
	bool bIsInit = false;

public:
	UFUNCTION(BlueprintPure, Category = "Database")
	FCraftRecipe GetRecipeData(FName RecipeID) const;
	UFUNCTION(BlueprintPure, Category = "Database")
	URecipeDataAsset* GetRecipeByID(FName RecipeID) const;
	UFUNCTION(BlueprintPure, Category = "Database")
	TArray<URecipeDataAsset*> GetAllRecipe() const { return AllRecipe; }
	UFUNCTION(BlueprintCallable, Category = "Database")
	void InitDB();
	UFUNCTION(BlueprintPure, Category = "Database")
	bool HasRecipe(FName RecipeID) const;
	UFUNCTION(BlueprintPure, Category = "Database")
	bool IsInit() const { return bIsInit; }

	UFUNCTION(BlueprintPure, Category = "Database")
	TArray<URecipeDataAsset*> GetFurnaceRecipes() const;
	UFUNCTION(BlueprintPure, Category = "Database")
	TArray<URecipeDataAsset*> GetAssemblerRecipes() const;
};
