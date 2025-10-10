// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/ItemDataAsset.h"
#include "ItemDatabase.generated.h"

/**
 * 
 */

UCLASS()
class OUTPOST_ARCHITECT_API UItemDatabase : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Database")
	TArray<UItemDataAsset*> AllItem;

protected:
	UPROPERTY(Transient)
	TMap<FName, UItemDataAsset*> ItemMap;
	UPROPERTY(Transient)
	bool bIsInit = false;

public:
	UFUNCTION(BlueprintPure, Category = "Database")
	FItemData GetItemData(FName ItemID) const;
	UFUNCTION(BlueprintPure, Category = "Database")
	UItemDataAsset* GetItemByID(FName ItemID) const;
	UFUNCTION(BlueprintPure, Category = "Database")
	TArray<UItemDataAsset*> GetAllItem() const { return AllItem; }
	UFUNCTION(BlueprintCallable, Category = "Database")
	void InitDB();
	UFUNCTION(BlueprintPure, Category = "Database")
	bool HasItem(FName ItemID) const;
	UFUNCTION(BlueprintPure, Category = "Database")
	bool IsInit() const { return bIsInit; }
};
