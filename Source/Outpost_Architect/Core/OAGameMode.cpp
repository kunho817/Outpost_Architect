// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/OAGameMode.h"
#include "Core/OAWaveManager.h"
#include "BuildSystem/OAGridSystem.h"
#include "BuildSystem/BuildManager.h"

AOAGameMode::AOAGameMode()
{
	WaveMan = CreateDefaultSubobject<UOAWaveManager>(TEXT("WaveManager"));
	Grid = CreateDefaultSubobject<UOAGridSystem>(TEXT("GridSystem"));
	BuildMan = CreateDefaultSubobject<UBuildManager>(TEXT("BuildManager"));
}

void AOAGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (WaveMan)
	{
		WaveMan->OnWaveComplete.AddDynamic(this, &AOAGameMode::OnWaveComplete);
	}
}

void AOAGameMode::OnWaveComplete(int32 WaveNum)
{
	UE_LOG(LogTemp, Log, TEXT("Wave %d completed!"), WaveNum);
}

void AOAGameMode::Victory()
{
	UE_LOG(LogTemp, Log, TEXT("Ending!"));
}

void AOAGameMode::GameOver()
{
	UE_LOG(LogTemp, Log, TEXT("Game Over..."));
}

void AOAGameMode::OnBaseCoreDestroy()
{
	GameOver();
}