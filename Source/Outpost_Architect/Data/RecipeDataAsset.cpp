// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/RecipeDataAsset.h"

FCraftRecipe URecipeDataAsset::ToRecipeData() const
{
	FCraftRecipe Recipe;

	Recipe.RecipeID = RecipeID;
	Recipe.Input = Input;
	Recipe.Output = Output;
	Recipe.RequireFuel = ReqFuel;
	Recipe.CraftTime = CraftTime;

	return Recipe;
}

FPrimaryAssetId URecipeDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("RecipeData", GetFName());
}
