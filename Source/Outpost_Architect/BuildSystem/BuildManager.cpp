// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/BuildManager.h"
#include "Building/OABuildingBase.h"
#include "Core/OAGameMode.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "PaperSpriteComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UBuildManager::UBuildManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	BuildModeActive = false;
	Ghost = nullptr;
	MaxDist = 1000.0f;
	CanPlace = false;
}


// Called when the game starts
void UBuildManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (UWorld* World = GetWorld()) OwnerCon = World->GetFirstPlayerController();

	AOAGameMode* GM = Cast<AOAGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM) Grid = GM->GetGrid();

	if (!Grid && GM) {
		Grid = NewObject<UOAGridSystem>(GM);
		Grid->RegisterComponent();
	}
}


// Called every frame
void UBuildManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (BuildModeActive && Ghost) UpdateGhostPos();
}

void UBuildManager::BuildModeOn(TSubclassOf<AOABuildingBase> BuildingClass)
{
	if (!BuildingClass) return;

	SelectBuildingClass = BuildingClass;
	BuildModeActive = true;

	CreateGhost();
	OnBuildModeChange.Broadcast(true);
}

void UBuildManager::BuildModeOff()
{
	BuildModeActive = false;
	SelectBuildingClass = nullptr;

	DestroyGhost();
	OnBuildModeChange.Broadcast(false);
}

void UBuildManager::UpdateGhostPos()
{
	if (!Ghost || !Grid) return;

	FVector MousePos = GetMousePos();

	FVector SnappedPos = Grid->SnapToGrid(MousePos);
	CurrPlaceLoc = SnappedPos;
	CurrCoord = Grid->WorldToGrid(SnappedPos);

	Ghost->SetActorLocation(SnappedPos);

	CanPlace = CanPlaceBuilding();

	UpdateGhostMat();
	OnPlacementChange.Broadcast(CanPlace, CurrPlaceLoc);
}

bool UBuildManager::TryPlaceBuilding()
{
	if (!CanPlaceBuilding()) return false;

	AOABuildingBase* Building = SelectBuildingClass->GetDefaultObject<AOABuildingBase>();
	FBuildCost BCost = Building->GetBCost_Implementation();

	if (!HasEnoughResource(BCost)) return false;

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AOABuildingBase* NBuilding = GetWorld()->SpawnActor<AOABuildingBase>(SelectBuildingClass, CurrPlaceLoc, FRotator::ZeroRotator, SpawnParam);

	if (NBuilding) {
		ConsumeResource(BCost);

		FVector BuildingSize = NBuilding->GetBuildingSize();
		int32 W = FMath::CeilToInt(BuildingSize.X / Grid->GetGridSize());
		int32 H = FMath::CeilToInt(BuildingSize.Y / Grid->GetGridSize());

		for (int32 X = 0; X < W; X++) {
			for (int32 Y = 0; Y < H; Y++) {
				FGridCoord OccupCoord(CurrCoord.X + X, CurrCoord.Y + Y);
				Grid->OccupyGrid(OccupCoord, NBuilding);
			}
		}
		NBuilding->StartConstruct();

		return true;
	}

	return false;
}

bool UBuildManager::CanPlaceBuilding()
{
	if (!Grid || !Ghost) return false;

	FVector BuildingSize = Ghost->GetBuildingSize();
	if (!Grid->CanPlace(CurrCoord, BuildingSize)) return false;

	if (OwnerCon) {
		APawn* Player = OwnerCon->GetPawn();
		if (Player) {
			float Dist = FVector::Dist(Player->GetActorLocation(), CurrPlaceLoc);
			if (Dist > MaxDist) return false;
		}
	}

	return true;
}

void UBuildManager::DestroyBuildingAtLocation()
{
	if (!Grid) return;

	FVector MousePos = GetMousePos();

	FGridCoord Coord = Grid->WorldToGrid(MousePos);
	AActor* Building = Grid->GetBuilding(Coord);

	if (Building && Building->GetClass()->ImplementsInterface(UBuildable::StaticClass())) {
		AOABuildingBase* OBuilding = Cast<AOABuildingBase>(Building);
		if (OBuilding) {
			FVector BuildingSize = OBuilding->GetBuildingSize();
			int32 W = FMath::CeilToInt(BuildingSize.X / Grid->GetGridSize());
			int32 H = FMath::CeilToInt(BuildingSize.Y / Grid->GetGridSize());

			FVector BuildLoc = Building->GetActorLocation();
			FGridCoord StartCoord = Grid->WorldToGrid(BuildLoc);

			for (int32 X = 0; X < W; X++) {
				for (int32 Y = 0; Y < H; Y++) {
					FGridCoord FreeCoord(CurrCoord.X + X, CurrCoord.Y + Y);
					Grid->FreeGrid(FreeCoord);
				}
			}
		}

		IBuildable::Execute_DestoryBuliding(Building);
	}
}

bool UBuildManager::HasEnoughResource(FBuildCost BCost)
{
	return true;
}

void UBuildManager::ConsumeResource(FBuildCost BCost)
{
}

void UBuildManager::CreateGhost()
{
	if (!SelectBuildingClass) return;

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Ghost = GetWorld()->SpawnActor<AOABuildingBase>(SelectBuildingClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParam);

	if (Ghost) {
		Ghost->SetActorEnableCollision(false);
	}
}

void UBuildManager::DestroyGhost()
{
	if (Ghost) {
		Ghost->Destroy();
		Ghost = nullptr;
	}
}

void UBuildManager::UpdateGhostMat()
{
	if (!Ghost) return;

	UPaperSpriteComponent* Sprite = Ghost->FindComponentByClass<UPaperSpriteComponent>();
	if (Sprite) {
		if (CanPlace) Sprite->SetSpriteColor(FLinearColor(0.0f, 1.0f, 0.0f, 0.5f));
		else Sprite->SetSpriteColor(FLinearColor(1.0f, 0.0f, 0.0f, 0.5f));
	}
}

FVector UBuildManager::GetMousePos()
{
	if (!OwnerCon) return FVector::ZeroVector;

	FVector WorldLoc, WorldDir;
	OwnerCon->DeprojectMousePositionToWorld(WorldLoc, WorldDir);

	float T = -WorldLoc.Y / WorldDir.Y;
	FVector Intersect = WorldLoc + (WorldDir * T);

	return Intersect;
}
