// Fill out your copyright notice in the Description page of Project Settings.


#include "OASpawnPoint.h"
#include "Components/BillboardComponent.h"

// Sets default values
AOASpawnPoint::AOASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Icon = CreateDefaultSubobject<UBillboardComponent>(TEXT("Icon"));
	RootComponent = Icon;

	SpawnType = EEnemyMoveType::Normal;

	Tags.Add(FName("SpawnPoint"));

}

// Called when the game starts or when spawned
void AOASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

