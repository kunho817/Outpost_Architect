// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/OAGameState.h"

AOAGameState::AOAGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOAGameState::AddMaxPower(float AdditonalPower)
{
	MaxPower += AdditonalPower;
}

void AOAGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameTime += DeltaTime;
}