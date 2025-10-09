// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Core/OACore.h"
#include "Core/OAGameMode.h"
#include "Core/OAWaveManager.h"
#include "Kismet/GameplayStatics.h"

AOACore::AOACore()
{
	BType = EBuildingType::Core;

	BName = FText::FromString(TEXT("Base Core"));
	BDesc = FText::FromString(TEXT("Bearing a beating heart, Must move forward."));

	MaxHealth = 5000.0f;
	CurrHealth = MaxHealth;

}

void AOACore::BeginPlay()
{
	Super::BeginPlay();

	GM = Cast<AOAGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM) {

		UOAWaveManager* WaveMan = GM->GetWaveMan();
		if (WaveMan) WaveMan->RegisterCore(this);
	}
}

void AOACore::Die_Implementation()
{
	Super::Die_Implementation();

	if (GM) GM->GameOver();
}