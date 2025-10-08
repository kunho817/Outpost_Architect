// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/OAGridSystem.h"

// Sets default values for this component's properties
UOAGridSystem::UOAGridSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	GridSize = 100.0f;
	SnapHeight = 0.0f;
}


// Called when the game starts
void UOAGridSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

FGridCoord UOAGridSystem::WorldToGrid(FVector WorldLoc)
{
	int32 X = FMath::RoundToInt(WorldLoc.X / GridSize);
	int32 Y = FMath::RoundToInt(WorldLoc.Y / GridSize);

	return FGridCoord(X, Y);
}

FVector UOAGridSystem::GridToWorld(FGridCoord Coord)
{
	float X = Coord.X * GridSize;
	float Y = Coord.Y * GridSize;
	float Z = SnapHeight;

	return FVector(X, Y, Z);
}

FVector UOAGridSystem::SnapToGrid(FVector WorldLoc)
{
	FGridCoord Coord = WorldToGrid(WorldLoc);
	return GridToWorld(Coord);
}

bool UOAGridSystem::IsOccupy(FGridCoord Coord)
{
	return OccupiedGrid.Contains(Coord);
}

bool UOAGridSystem::CanPlace(FGridCoord Coord, FVector Size)
{
	int32 Width = FMath::CeilToInt(Size.X / GridSize);
	int32 Height = FMath::CeilToInt(Size.Y / GridSize);

	for (int32 X = 0; X < Width; X++) {
		for (int32 Y = 0; Y < Height; Y++) {
			FGridCoord CheckCoord(Coord.X + X, Coord.Y + Y);
			if (IsOccupy(CheckCoord)) return false;
		}
	}

	return true;
}

void UOAGridSystem::OccupyGrid(FGridCoord Coord, AActor* Building)
{
	OccupiedGrid.Add(Coord, Building);
}

void UOAGridSystem::FreeGrid(FGridCoord Coord)
{
	OccupiedGrid.Remove(Coord);
}

AActor* UOAGridSystem::GetBuilding(FGridCoord Coord)
{
	if (AActor** Found = OccupiedGrid.Find(Coord)) return *Found;

	return nullptr;
}

