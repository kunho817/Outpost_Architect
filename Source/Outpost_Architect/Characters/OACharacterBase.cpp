// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/OACharacterBase.h"

// Sets default values
AOACharacterBase::AOACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

