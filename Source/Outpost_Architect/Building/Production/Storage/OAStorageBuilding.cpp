// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Production/Storage/OAStorageBuilding.h"
#include "Inventory/OAInventoryComponent.h"
#include "OAStorageBuilding.h"

AOAStorageBuilding::AOAStorageBuilding()
{
	BType = EBuildingType::Production;

	Inv = CreateDefaultSubobject<UOAInventoryComponent>(TEXT("StorageInventory"));
	Inv->SetInvType(EInventoryType::Storage);
	Inv->SetMaxCap(300);
}

void AOAStorageBuilding::BeginPlay()
{
	Super::BeginPlay();
}

void AOAStorageBuilding::Interact_Implementation(AActor* Interactor)
{
	if (!IsOperation()) return;
	UOAInventoryComponent* PInv = Interactor->FindComponentByClass<UOAInventoryComponent>();
	if (!PInv || PInv->GetInvType() != EInventoryType::Player) return;

	bool Succed = PInv->TTStorage(Inv);
}

bool AOAStorageBuilding::CanInteract_Implementation(AActor* Interactor) const
{
	return IsOperation();
}

FText AOAStorageBuilding::GetInteractionText_Implementation() const
{
	if (!IsOperation()) {
		return FText::FromString(TEXT("Not Operational"));
	}

	return FText::Format(
		FText::FromString(TEXT("Storage ({0}/{1})")),
		FText::AsNumber(Inv->GetTotalAmount()),
		FText::AsNumber(Inv->GetMaxCap())
	);
}
