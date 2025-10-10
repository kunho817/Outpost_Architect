// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ItemDatabase.h"

UItemDataAsset* UItemDatabase::GetItemByID(FName ItemID) const
{
	if (!bIsInit) return nullptr;

	if (UItemDataAsset* const* Found = ItemMap.Find(ItemID)) return *Found;
	return nullptr;
}

void UItemDatabase::InitDB()
{
	if (bIsInit) return;

	ItemMap.Empty();

	for (UItemDataAsset* Item : AllItem) {
		if (Item && !Item->ItemID.IsNone()) {
			if (ItemMap.Contains(Item->ItemID)) continue;

			ItemMap.Add(Item->ItemID, Item);
		}
	}

	bIsInit = true;
}

FItemData UItemDatabase::GetItemData(FName ItemID) const
{
	UItemDataAsset* Item = GetItemByID(ItemID);
	if (Item) return Item->ToItemData();

	return FItemData();
}

bool UItemDatabase::HasItem(FName ItemID) const
{
	if (!bIsInit) return false;
	return ItemMap.Contains(ItemID);
}