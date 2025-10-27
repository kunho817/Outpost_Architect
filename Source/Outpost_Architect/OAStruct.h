// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OAStruct.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FItemData {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MaxStack = 50;
};

USTRUCT(BlueprintType)
struct FSlot{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemData Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Stack = 0;

	bool IsEmpty() const { return Stack <= 0; }
	bool IsFull() const { return Stack >= Item.MaxStack; }
};

USTRUCT(BlueprintType)
struct FRecipeIngredient{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Require = 1;
};

USTRUCT(BlueprintType)
struct FCraftRecipe{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RecipeID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRecipeIngredient> Input;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRecipeIngredient Output;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CraftTime = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool RequireFuel = false;
};

USTRUCT(BlueprintType)
struct FDamage{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<AActor> Instigator = nullptr;
};

USTRUCT(BlueprintType)
struct FBuildCost{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRecipeIngredient> RequireItem;
};

class OUTPOST_ARCHITECT_API OAStruct
{
public:
	OAStruct();
	~OAStruct();
};
