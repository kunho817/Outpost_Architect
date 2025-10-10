// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ItemDataAsset.h"

FItemData UItemDataAsset::ToItemData() const
{
	FItemData D;
	D.ItemID = ItemID;
	D.ItemName = ItemName;
	D.Icon = Icon;
	D.MaxStack = MaxStack;
	return D;
}

FPrimaryAssetId UItemDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("ItemData", GetFName());
}
