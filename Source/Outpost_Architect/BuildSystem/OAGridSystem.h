// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OAGridSystem.generated.h"

USTRUCT(BlueprintType)
struct FGridCoord
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 X;
	UPROPERTY(BlueprintReadWrite)
	int32 Y;

	FGridCoord() : X(0), Y(0) {}
	FGridCoord(int32 IX, int32 IY) : X(IX), Y(IY) {}

	bool operator==(const FGridCoord& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	bool operator!=(const FGridCoord& Other) const
	{
		return !(*this == Other);
	}

	friend uint32 GetTypeHash(const FGridCoord& Coord)
	{
		return HashCombine(GetTypeHash(Coord.X), GetTypeHash(Coord.Y));
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OUTPOST_ARCHITECT_API UOAGridSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOAGridSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float GridSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float SnapHeight;

	UPROPERTY()
	TMap<FGridCoord, AActor*> OccupiedGrid;

public:	
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FGridCoord WorldToGrid(FVector WorldLoc);
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FVector GridToWorld(FGridCoord Coord);
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FVector SnapToGrid(FVector WorldLoc);

	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool IsOccupy(FGridCoord Coord);
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool CanPlace(FGridCoord Coord, FVector Size);
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void OccupyGrid(FGridCoord Coord, AActor* Building);
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void FreeGrid(FGridCoord Coord);

	UFUNCTION(BlueprintPure, Category = "Grid")
	float GetGridSize() const { return GridSize; }
	UFUNCTION(BlueprintPure, Category = "Grid")
	AActor* GetBuilding(FGridCoord Coord);
		
};
