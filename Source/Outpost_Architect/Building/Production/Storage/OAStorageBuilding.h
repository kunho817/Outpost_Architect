// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/OABuildingBase.h"
#include "Interfaces/Interactable.h"
#include "OAStorageBuilding.generated.h"

/**
 * 
 */
class UOAInventoryComponent;

UCLASS()
class OUTPOST_ARCHITECT_API AOAStorageBuilding : public AOABuildingBase, public IInteractable
{
	GENERATED_BODY()

public:
	AOAStorageBuilding();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Storage")
	UOAInventoryComponent* Inv = nullptr;

public:
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual bool CanInteract_Implementation(AActor* Interactor) const override;
	virtual FText GetInteractionText_Implementation() const override;

	UFUNCTION(BlueprintPure, Category = "Storage")
	UOAInventoryComponent* GetInv() const { return Inv; }
};
