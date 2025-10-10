// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/OAInventoryComponent.h"
#include "Core/OAGameMode.h"
#include "Data/ItemDataAsset.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UOAInventoryComponent::UOAInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UOAInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (InvType == EInventoryType::Player && Slots.Num() == 0) Slots.SetNum(1);
}

UItemDataAsset* UOAInventoryComponent::GetItemAsset(FName ItemID) const
{
	AOAGameMode* GM = Cast<AOAGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM) return GM->GetItemAsset(ItemID);
	return nullptr;
}

FItemData UOAInventoryComponent::GetItemData(FName ItemID) const
{
	UItemDataAsset* Asset = GetItemAsset(ItemID);
	if (Asset) return Asset->ToItemData();

	return FItemData();
}

FSlot* UOAInventoryComponent::FindSlot(FName ItemID)
{
	for (FSlot& Slot : Slots) {
		if (!Slot.IsEmpty() && Slot.Item.ItemID == ItemID) return &Slot;
	}
	return nullptr;
}

const FSlot* UOAInventoryComponent::FindSlot(FName ItemID) const
{
	for (const FSlot& Slot : Slots) {
		if (!Slot.IsEmpty() && Slot.Item.ItemID == ItemID) return &Slot;
	}
	return nullptr;
}

void UOAInventoryComponent::BroadcastInventoryChange(FName ItemID, int32 NewAmount)
{
	OnInventoryChange.Broadcast(ItemID, NewAmount);
}

void UOAInventoryComponent::SetInvType(EInventoryType Type)
{
	InvType = Type;
}

void UOAInventoryComponent::SetMaxCap(int32 NewCap)
{
	MaxCapacity = NewCap;
}

bool UOAInventoryComponent::CanAddItem(FName ItemID, int32 Amount) const
{
	if (ItemID.IsNone() || Amount <= 0) return false;
	
	FItemData ItemData = GetItemData(ItemID);
	if (ItemData.ItemID.IsNone()) return false;

	if (InvType == EInventoryType::Player) {
		if (Slots.Num() == 0) return true;

		const FSlot& Slot = Slots[0];
		if (Slot.IsEmpty()) return true;
		if (Slot.Item.ItemID == ItemID) {
			int32 MaxStack = FMath::Min(ItemData.MaxStack, MaxCapacity);
			return Slot.Stack < MaxStack;
		}

		return false;
	}

	if (InvType == EInventoryType::Storage) return GetAvailSlot() >= Amount;
	return false;
}

int32 UOAInventoryComponent::AddItem(FName ItemID, int32 Amount)
{
	if (ItemID.IsNone() || Amount <= 0) return 0;
	
	FItemData ItemData = GetItemData(ItemID);

	int32 TotalAdded = 0;
	int32 Remaining = Amount;

	if (InvType == EInventoryType::Player) {
		if (Slots.Num() == 0) Slots.SetNum(1);

		FSlot& PlayerSlot = Slots[0];
		if (PlayerSlot.IsEmpty()) {
			int32 AddAmount = FMath::Min(Remaining, ItemData.MaxStack);
			AddAmount = FMath::Min(AddAmount, MaxCapacity);

			PlayerSlot.Item = ItemData;
			PlayerSlot.Stack = AddAmount;
			TotalAdded = AddAmount;
		}
		else if (PlayerSlot.Item.ItemID == ItemID) {
			int32 AvailStack = FMath::Min(ItemData.MaxStack, MaxCapacity) - PlayerSlot.Stack;
			int32 AddAmount = FMath::Min(Remaining, AvailStack);

			PlayerSlot.Stack += AddAmount;
			TotalAdded = AddAmount;
		}
		else {
			return 0;
		}

		BroadcastInventoryChange(ItemID, PlayerSlot.Stack);
		return TotalAdded;
	}

	if (InvType == EInventoryType::Storage) {
		int32 TotalStack = GetTotalAmount();
		int32 AvailStack = MaxCapacity - TotalStack;

		if (AvailStack <= 0) return 0;

		Remaining = FMath::Min(Remaining, AvailStack);

		FSlot* ExistSlot = FindSlot(ItemID);

		while (Remaining > 0) {
			if (ExistSlot && !ExistSlot->IsFull()) {
				int32 SlotSpace = ItemData.MaxStack - ExistSlot->Stack;
				int32 AddAmount = FMath::Min(Remaining, SlotSpace);

				ExistSlot->Stack += AddAmount;
				TotalAdded += AddAmount;
				Remaining -= AddAmount;
			}
			else {
				int32 AddAmount = FMath::Min(Remaining, ItemData.MaxStack);

				FSlot NewSlot;
				NewSlot.Item = ItemData;
				NewSlot.Stack = AddAmount;
				Slots.Add(NewSlot);

				TotalAdded += AddAmount;
				Remaining -= AddAmount;

				ExistSlot = &Slots.Last();
			}
		}

		BroadcastInventoryChange(ItemID, GetItemAmount(ItemID));
		return TotalAdded;
	}

	return 0;
}

bool UOAInventoryComponent::RemoveItem(FName ItemID, int32 Amount)
{
	if (ItemID.IsNone() || Amount <= 0) return false;

	int32 CurrAmount = GetItemAmount(ItemID);
	if (CurrAmount < Amount) return false;

	int32 Remaining = Amount;

	for (int32 i = Slots.Num() - 1; i >= 0; i--) {
		FSlot& Slot = Slots[i];

		if (Slot.Item.ItemID == ItemID) {
			int32 RemoveAmount = FMath::Min(Slot.Stack, Remaining);
			Slot.Stack -= RemoveAmount;
			Remaining -= RemoveAmount;

			if (Slot.Stack <= 0) Slots.RemoveAt(i);

			if (Remaining <= 0) break;
		}
	}
	
	BroadcastInventoryChange(ItemID, GetItemAmount(ItemID));
	return true;
}

int32 UOAInventoryComponent::GetItemAmount(FName ItemID) const
{
	int32 TotalAmount = 0;

	for (const FSlot& Slot : Slots) {
		if (!Slot.IsEmpty()) TotalAmount += Slot.Stack;
	}

	return TotalAmount;
}

int32 UOAInventoryComponent::GetTotalAmount() const
{
	int32 Total = 0;
	for (const FSlot& Slot : Slots) {
		if (!Slot.IsEmpty()) Total += Slot.Stack;
	}
	return Total;
}

int32 UOAInventoryComponent::GetAvailSlot() const
{
	return MaxCapacity - GetTotalAmount();
}

void UOAInventoryComponent::ClearInv()
{
	Slots.Empty();

	if (InvType == EInventoryType::Player) Slots.SetNum(1);

	BroadcastInventoryChange(NAME_None, 0);
}

FName UOAInventoryComponent::GetCurrentItemID() const
{
	if (InvType != EInventoryType::Player || Slots.Num() == 0) return NAME_None;

	const FSlot& Slot = Slots[0];
	return Slot.IsEmpty() ? NAME_None : Slot.Item.ItemID;
}

bool UOAInventoryComponent::TTStorage(UOAInventoryComponent* OtherInv)
{
	if (!OtherInv) return false;
	if (InvType != EInventoryType::Player) return false;
	if (OtherInv->InvType != EInventoryType::Storage) return false;
	if (Slots.Num() == 0 || Slots[0].IsEmpty()) return false;

	FSlot& Slot = Slots[0];
	FName ItemID = Slot.Item.ItemID;
	int32 Amount = Slot.Stack;

	int32 TAmount = OtherInv->AddItem(ItemID, Amount);

	if (TAmount > 0) {
		RemoveItem(ItemID, TAmount);
		return true;
	}

	return false;
}

bool UOAInventoryComponent::HasItem(FName ItemID) const
{
	return GetItemAmount(ItemID) > 0;
}
