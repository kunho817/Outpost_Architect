// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OAStruct.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class OUTPOST_ARCHITECT_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText Desc;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UTexture2D* Icon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 MaxStack = 100;

	UFUNCTION(BlueprintPure, Category = "Item")
	FItemData ToItemData() const;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
