// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/OAGameMode.h"
#include "Core/OAWaveManager.h"
#include "Inventory/OAInventoryComponent.h"
#include "BuildSystem/OAGridSystem.h"
#include "BuildSystem/BuildManager.h"

AOAGameMode::AOAGameMode()
{
	WaveMan = CreateDefaultSubobject<UOAWaveManager>(TEXT("WaveManager"));
	Grid = CreateDefaultSubobject<UOAGridSystem>(TEXT("GridSystem"));
	BuildMan = CreateDefaultSubobject<UBuildManager>(TEXT("BuildManager"));
}

UItemDataAsset* AOAGameMode::GetItemAsset(FName ItemID) const
{
	if (!ItemDB) return nullptr;
	return ItemDB->GetItemByID(ItemID);
}

FItemData AOAGameMode::GetItemData(FName ItemID) const
{
	if (!ItemDB) return FItemData();
	return ItemDB->GetItemData(ItemID);
}

URecipeDataAsset* AOAGameMode::GetRecipeAsset(FName RecipeID) const
{
	if (!RecipeDB) return nullptr;
	return RecipeDB->GetRecipeByID(RecipeID);
}

FCraftRecipe AOAGameMode::GetRecipeData(FName RecipeID) const
{
	if (!RecipeDB) return FCraftRecipe();
	return RecipeDB->GetRecipeData(RecipeID);
}

void AOAGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (WaveMan) WaveMan->OnWaveComplete.AddDynamic(this, &AOAGameMode::OnWaveComplete);

	if (ItemDB) ItemDB->InitDB();
	if (RecipeDB) RecipeDB->InitDB();

	if (UWorld* World = GetWorld()) {
		APawn* Player = World->GetFirstPlayerController()->GetPawn();
		if (Player) {
			UOAInventoryComponent* PInv = Player->FindComponentByClass<UOAInventoryComponent>();
			if (PInv && BuildMan) BuildMan->SetPlayerInventory(PInv);
		}
	}
}

void AOAGameMode::OnWaveComplete(int32 WaveNum)
{
	UE_LOG(LogTemp, Log, TEXT("Wave %d completed!"), WaveNum);
}

void AOAGameMode::Victory()
{
	UE_LOG(LogTemp, Log, TEXT("Ending!"));
}

void AOAGameMode::GameOver()
{
	UE_LOG(LogTemp, Log, TEXT("Game Over..."));
}

void AOAGameMode::OnBaseCoreDestroy()
{
	GameOver();
}