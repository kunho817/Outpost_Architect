// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OAPlayerCharacter.h"

// Sets default values
AOAPlayerCharacter::AOAPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOAPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOAPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOAPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

