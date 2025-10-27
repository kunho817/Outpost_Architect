// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Production/OAProductionBuildingBase.h"
#include "Inventory/OAInventoryComponent.h"
#include "Core/OAGameMode.h"
#include "Kismet/GameplayStatics.h"


AOAProductionBuildingBase::AOAProductionBuildingBase()
{
	PrimaryActorTick.bCanEverTick = true;

	InputInv = CreateDefaultSubobject<UOAInventoryComponent>(TEXT("InputInventory"));
	OutputInv = CreateDefaultSubobject<UOAInventoryComponent>(TEXT("OutputInventory"));

	InputSlotCount = 1;
	OutputSlotCount = 1;

	CurrentState = EProductionState::Idle;
	CraftProgress = 0.0f;
	bHasActiveRecipe = false;
}

void AOAProductionBuildingBase::BeginPlay()
{
	Super::BeginPlay();

	if (InputInv) {
		InputInv->SetInvType(EInventoryType::Storage);
		InputInv->SetMaxCap(InputSlotCount * 20);
	}
	if(OutputInv) {
		OutputInv->SetInvType(EInventoryType::Storage);
		OutputInv->SetMaxCap(OutputSlotCount * 20);
	}

	CurrentState = EProductionState::Idle;
	CraftProgress = 0.0f;
	bHasActiveRecipe = false;
}

void AOAProductionBuildingBase::Tick(float DeltaTick)
{
	Super::Tick(DeltaTick);

	if (CurrentState == EProductionState::Processing) {
		if (!bHasActiveRecipe) {
			CurrentState = EProductionState::Idle;
			OnProductionStateChanged(CurrentState);
			return;
		}

		if (!GenPower) {
			PauseProduction();
			return;
		}

		TickProduction(DeltaTick);
	}
}

void AOAProductionBuildingBase::StartProduction()
{
	if (!CanStartProduction()) return;
	if (!ConsumeInput()) return;

	CraftProgress = 0.0f;
	CurrentState = EProductionState::Processing;

	OnProductionStart();
	OnProductionStateChanged(CurrentState);
}

void AOAProductionBuildingBase::PauseProduction()
{
	if (CurrentState != EProductionState::Processing) return;

	CurrentState = EProductionState::Paused;

	OnProductionPause();
	OnProductionStateChanged(CurrentState);
}

void AOAProductionBuildingBase::ResumeProduction()
{
	if (CurrentState != EProductionState::Paused) return;
	if (!bHasActiveRecipe || !GenPower) return;

	CurrentState = EProductionState::Processing;

	OnProductionStateChanged(CurrentState);
}

bool AOAProductionBuildingBase::SetRecipe(FName RecipeID)
{
	if (CurrentState == EProductionState::Processing) return false;

	AOAGameMode* GM = Cast<AOAGameMode>(UGameplayStatics::GetGameMode(this));
	if (!GM || !GM->GetRecipeDB()) return false;

	FCraftRecipe Recipe = GM->GetRecipeDB()->GetRecipeData(RecipeID);

	CurrRecipe = Recipe;
	bHasActiveRecipe = true;
	CurrentState = EProductionState::Idle;
	CraftProgress = 0.0f;

	return true;
	
}

void AOAProductionBuildingBase::ClearRecipe()
{
	CurrRecipe = FCraftRecipe();
	bHasActiveRecipe = false;
	CurrentState = EProductionState::Idle;
	CraftProgress = 0.0f;

	OnProductionStateChanged(CurrentState);
}

bool AOAProductionBuildingBase::CanStartProduction() const
{
	if (!bHasActiveRecipe || CurrRecipe.RecipeID.IsNone()) return false;
	if (CurrentState == EProductionState::Processing) return false;
	if (!GenPower) return false;
	if (!HasReqInput()) return false;
	if (!HasOutputSpace()) return false;
	return true;
}

bool AOAProductionBuildingBase::HasReqInput() const
{
	if (!InputInv || !bHasActiveRecipe) return false;

	for (const FRecipeIngredient& Ing : CurrRecipe.Input) {
		
		int32 AvailAmount = InputInv->GetItemAmount(Ing.ItemID);
		if (AvailAmount < Ing.Require) return false;
	}

	return true;
}

bool AOAProductionBuildingBase::HasOutputSpace() const
{
	if (!OutputInv || !bHasActiveRecipe) return false;

	if (!OutputInv->CanAddItem(CurrRecipe.Output.ItemID, CurrRecipe.Output.Require)) return false;

	return true;
}

void AOAProductionBuildingBase::TickProduction(float DeltaTime)
{
	if (!bHasActiveRecipe || CurrRecipe.RecipeID.IsNone()) {
		CurrentState = EProductionState::Idle;
		return;
	}

	float ProgIncre = DeltaTime / CurrRecipe.CraftTime;
	CraftProgress += ProgIncre;

	CraftProgress = FMath::Clamp(CraftProgress, 0.0f, 1.0f);

	if (CraftProgress >= 1.0f) {
		if (ProduceOutput()) {
			CurrentState = EProductionState::Complete;

			OnProductionComplete();
			OnProductionStateChanged(CurrentState);

			CraftProgress = 0.0f;
		}
		else {
			CurrentState = EProductionState::Paused;

			OnProductionPause();
			OnProductionStateChanged(CurrentState);
		}
	}
}

bool AOAProductionBuildingBase::ConsumeInput()
{
	if (!InputInv || !bHasActiveRecipe) return false;
	if (!HasReqInput()) return false;

	for (const FRecipeIngredient& Ing : CurrRecipe.Input) {

		bool bRemove = InputInv->RemoveItem(Ing.ItemID, Ing.Require);

		if (!bRemove) return false;
	}

	return true;
}

bool AOAProductionBuildingBase::ProduceOutput()
{
	if (!OutputInv || !bHasActiveRecipe) return false;
	if (!HasOutputSpace()) return false;

	const FRecipeIngredient& Prod = CurrRecipe.Output;

	int AmountAdd = OutputInv->AddItem(Prod.ItemID, Prod.Require);
	if (AmountAdd < Prod.Require) return false;

	return true;
}

void AOAProductionBuildingBase::OnProductionStateChanged(EProductionState NewState)
{
	OnProductionStateChange(NewState);
}

void AOAProductionBuildingBase::OnProductionStart_Implementation()
{
}

void AOAProductionBuildingBase::OnProductionComplete_Implementation()
{
}

void AOAProductionBuildingBase::OnProductionPause_Implementation()
{
}

void AOAProductionBuildingBase::OnProductionStateChange_Implementation(EProductionState NewState)
{
}
