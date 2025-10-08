// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OAStruct.h"
#include "OnDamage.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOnDamage : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OUTPOST_ARCHITECT_API IOnDamage
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void TakeDamage(const FDamage& DInfo);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	float GetCurrHealth() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	float GetMaxHealth() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	bool Alive() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void Die();

};
