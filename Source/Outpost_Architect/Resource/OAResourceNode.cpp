// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource/OAResourceNode.h"
#include "Inventory/OAInventoryComponent.h"
#include "PaperSpriteComponent.h"

// Sets default values
AOAResourceNode::AOAResourceNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	RootComponent = MeshComp;

}

// Called when the game starts or when spawned
void AOAResourceNode::BeginPlay()
{
	Super::BeginPlay();
	
	RemainAmount = TotalAmount;
}

void AOAResourceNode::Interact_Implementation(AActor* Interactor)
{
	if (!Interactor) return;

	float CurrTime = GetWorld()->GetTimeSeconds();
	if (CurrTime - PrevMiningTime < MineCool) return;

	if (IsDepleted()) return;
	UOAInventoryComponent* PInv = Interactor->FindComponentByClass<UOAInventoryComponent>();
	if (!PInv) return;

	if (PInv->GetInvType() != EInventoryType::Player) return;

	int32 PAvail = PInv->GetAvailSlot();
	int32 MineAmount = FMath::Min(PAvail, MiningAmount);

	if (MineAmount <= 0) return;

	int32 MinedAmount = Mine(MineAmount);

	if (MinedAmount > 0) {
		int32 AddAmount = PInv->AddItem(ItemID, MinedAmount);

		PrevMiningTime = CurrTime;

		if (IsDepleted()) OnNodeDepleted();
	}
}

bool AOAResourceNode::CanInteract_Implementation(AActor* Interactor) const
{
	if (IsDepleted()) return false;

	float CurrTime = GetWorld()->GetTimeSeconds();
	if (CurrTime - PrevMiningTime < MineCool) return false;

	return true;
}

FText AOAResourceNode::GetInteractionText_Implementation() const
{
	if (IsDepleted()) {
		return FText::FromString(TEXT("Depleted"));
	}

	return FText::Format(
		FText::FromString(TEXT("Mine {0} ({1}/{2})")),
		FText::FromName(ItemID),
		FText::AsNumber(RemainAmount),
		FText::AsNumber(TotalAmount)
	);
}

float AOAResourceNode::GetDepletionPer() const
{
	if (TotalAmount <= 0)return 1.0f;
	return 1.0f - (static_cast<float>(RemainAmount) / TotalAmount);
}

void AOAResourceNode::OnNodeDepleted_Implementation()
{
	Destroy();
}

int32 AOAResourceNode::Mine(int32 Request)
{
	int32 MAmount = FMath::Min(Request, RemainAmount);
	RemainAmount -= MAmount;
	return MAmount;
}
