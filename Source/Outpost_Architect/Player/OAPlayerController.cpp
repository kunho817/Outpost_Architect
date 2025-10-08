// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OaPlayerController.h"
#include "Core/OAGameMode.h"
#include "BuildSystem/BuildManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

AOaPlayerController::AOaPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	BuildMan = nullptr;
}

void AOaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AOAGameMode* GM = Cast<AOAGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM) {
		BuildMan = GM->GetBuildMan();
		if (BuildMan) BuildMan->SetPlayerCon(this);
	}
}

void AOaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {
		if (PlaceBuildingAction) EIC->BindAction(PlaceBuildingAction, ETriggerEvent::Triggered, this, &AOaPlayerController::TryPlaceBuild);
		if (CancelBuildingAction) EIC->BindAction(CancelBuildingAction, ETriggerEvent::Triggered, this, &AOaPlayerController::BuildModeOff);
		if (DestroyAction) EIC->BindAction(DestroyAction, ETriggerEvent::Triggered, this, &AOaPlayerController::TryDestroyBuilding);
	}
}

void AOaPlayerController::BuildModeOn(TSubclassOf<AOABuildingBase> BuildingClass)
{
	if (BuildMan) BuildMan->BuildModeOn(BuildingClass);
}

void AOaPlayerController::BuildModeOff()
{
	if (BuildMan) BuildMan->BuildModeOff();
}

void AOaPlayerController::TryPlaceBuild()
{
	if (BuildMan) BuildMan->TryPlaceBuilding();
}

void AOaPlayerController::TryDestroyBuilding()
{
	if (BuildMan) BuildMan->DestroyBuildingAtLocation();
}

bool AOaPlayerController::InBuildMode() const
{
	return BuildMan ? BuildMan->IsInBuildMode() : false;
}

