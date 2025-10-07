// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/OABuildingBase.h"

// Sets default values
AOABuildingBase::AOABuildingBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOABuildingBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOABuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

