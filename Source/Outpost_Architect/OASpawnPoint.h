// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OAEnum.h"
#include "OASpawnPoint.generated.h"

UCLASS()
class OUTPOST_ARCHITECT_API AOASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOASpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UBillboardComponent* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	EEnemyMoveType SpawnType;

public:	
	UFUNCTION(BlueprintPure, Category = "Spawn")
	EEnemyMoveType GetSpawnType() const { return SpawnType; }

};
