// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OAEnum.h"
#include "OAStruct.h"
#include "OAInventoryComponent.generated.h"

class UItemDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryChange, FName, ItemID, int32, NewAmount);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OUTPOST_ARCHITECT_API UOAInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOAInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	EInventoryType InvType = EInventoryType::Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxCapacity = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FSlot> Slots;

public:	
	UFUNCTION(BlueprintPure, Category = "Basic")
	EInventoryType GetInvType() const { return InvType; }
	UFUNCTION(BlueprintPure, Category = "Basic")
	int32 GetMaxCap() const { return MaxCapacity; }
	UFUNCTION(BlueprintCallable, Category = "Basic")
	void SetInvType(EInventoryType Type);
	UFUNCTION(BlueprintCallable, Category = "Basic")
	void SetMaxCap(int32 NewCap);
	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool CanAddItem(FName ItemID, int32 Amount) const;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 AddItem(FName ItemID, int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(FName ItemID, int32 Amount);
	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetItemAmount(FName ItemID) const;
	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetTotalAmount() const;
	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetAvailSlot() const;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClearInv();
	UFUNCTION(BlueprintPure, Category = "Inventory")
	TArray<FSlot> GetAllSlot() const { return Slots; }

	UFUNCTION(BlueprintPure, Category = "Inventory|Player")
	FName GetCurrentItemID() const;
	UFUNCTION(BlueprintCallable, Category = "Inventory|Player")
	bool TTStorage(UOAInventoryComponent* OtherInv);
	
	UFUNCTION(BlueprintPure, Category = "Inventory|Storage")
	bool HasItem(FName ItemID) const;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnInventoryChange OnInventoryChange;

protected:
	UItemDataAsset* GetItemAsset(FName ItemID) const;
	FItemData GetItemData(FName ItemID) const;

	FSlot* FindSlot(FName ItemID);
	const FSlot* FindSlot(FName ItemID) const;

	void BroadcastInventoryChange(FName ItemID, int32 NewAmount);
};
