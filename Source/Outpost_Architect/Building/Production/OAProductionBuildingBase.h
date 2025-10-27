// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/OABuildingBase.h"
#include "OAStruct.h"
#include "OAEnum.h"
#include "OAProductionBuildingBase.generated.h"

/**
 * 
 */
class UOAInventoryComponent;

UCLASS()
class OUTPOST_ARCHITECT_API AOAProductionBuildingBase : public AOABuildingBase
{
	GENERATED_BODY()
	
public:

	AOAProductionBuildingBase();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Production|Inventory")
	UOAInventoryComponent* InputInv;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Production|Inventory")
	UOAInventoryComponent* OutputInv;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Production|Config", meta = (ClampMin = "1", ClampMax = "20"))
	int32 InputSlotCount = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Production|Config", meta = (ClampMin = "1", ClampMax = "20"))
	int32 OutputSlotCount = 1;

public:

	virtual void Tick(float DeltaTick) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Production|State")
	EProductionState GetProdState() const { return CurrentState; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Production|State")
	float GetCraftProgress() const { return CraftProgress; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Production|State")
	const FCraftRecipe& GetCurrentRecipe() const { return CurrRecipe; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Production|State")
	bool HasActiveRecipe() const { return bHasActiveRecipe; }

	UFUNCTION(BlueprintCallable, Category = "Production|Control")
	void StartProduction();
	UFUNCTION(BlueprintCallable, Category = "Production|Control")
	void PauseProduction();
	UFUNCTION(BlueprintCallable, Category = "Production|Control")
	void ResumeProduction();

	UFUNCTION(BlueprintCallable, Category = "Production|Recipe")
	bool SetRecipe(FName RecipeID);
	UFUNCTION(BlueprintCallable, Category = "Production|Recipe")
	void ClearRecipe();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Production|Valid")
	bool CanStartProduction() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Production|Valid")
	bool HasReqInput() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Production|Valid")
	bool HasOutputSpace() const;

protected:

	virtual void TickProduction(float DeltaTime);
	virtual bool ConsumeInput();
	virtual bool ProduceOutput();
	virtual void OnProductionStateChanged(EProductionState NewState);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Production|State", meta = (AllowPrivateAccess = "true"))
	EProductionState CurrentState = EProductionState::Idle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Production|State", meta = (AllowPrivateAccess = "true"))
	FCraftRecipe CurrRecipe;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Production|State", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0"))
	float CraftProgress = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Production|State", meta = (AllowPrivateAccess = "true"))
	bool bHasActiveRecipe = false;


public:

	UFUNCTION(BlueprintNativeEvent, Category = "Production|Events")
	void OnProductionStart();
	virtual void OnProductionStart_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "Production|Events")
	void OnProductionComplete();
	virtual void OnProductionComplete_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "Production|Events")
	void OnProductionPause();
	virtual void OnProductionPause_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "Production|Events")
	void OnProductionStateChange(EProductionState NewState);
	virtual void OnProductionStateChange_Implementation(EProductionState NewState);
};
