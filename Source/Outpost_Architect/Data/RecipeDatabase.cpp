// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/RecipeDatabase.h"

FCraftRecipe URecipeDatabase::GetRecipeData(FName RecipeID) const
{
	URecipeDataAsset* Recipe = GetRecipeByID(RecipeID);
	if (Recipe) return Recipe->ToRecipeData();
	return FCraftRecipe();
}

URecipeDataAsset* URecipeDatabase::GetRecipeByID(FName RecipeID) const
{
	URecipeDataAsset* const* Recipe = RecipeMap.Find(RecipeID);
	if (Recipe) return *Recipe;
	return nullptr;
}

TArray<URecipeDataAsset*> URecipeDatabase::GetFurnaceRecipes() const
{
	TArray<URecipeDataAsset*> Temp;

	for (auto& Recipe : RecipeMap) {
		URecipeDataAsset* RT;
		RT = Recipe.Value;
		if (RT->ReqFuel) Temp.Add(RT);
	}

	return Temp;
}

TArray<URecipeDataAsset*> URecipeDatabase::GetAssemblerRecipes() const
{
	TArray<URecipeDataAsset*> Temp;

	for (auto& Recipe : RecipeMap) {
		URecipeDataAsset* RT;
		RT = Recipe.Value;
		if (!RT->ReqFuel) Temp.Add(RT);
	}

	return Temp;
}

bool URecipeDatabase::HasRecipe(FName RecipeID) const
{
	return RecipeMap.Contains(RecipeID);
}

void URecipeDatabase::InitDB() {
	if (bIsInit) return;

	RecipeMap.Empty();

	for (URecipeDataAsset* Item : AllRecipe) {
		if (Item && !Item->RecipeID.IsNone()) {
			if (RecipeMap.Contains(Item->RecipeID)) continue;

			RecipeMap.Add(Item->RecipeID, Item);
		}
	}

	bIsInit = true;
}