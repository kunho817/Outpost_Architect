// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/OAWaveManager.h"
#include "OASpawnPoint.h"
#include "Kismet/GameplayStatics.h"

UOAWaveManager::UOAWaveManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOAWaveManager::BeginPlay()
{
	Super::BeginPlay();

	CurrMapStayTime = 0.f;
	WaveStat = EWaveState::Waiting;

	RefreshSpawn();
}

void UOAWaveManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (WaveStat)
	{
	case EWaveState::Idle:
		CheckWaveTrigger(DeltaTime);
		break;

	case EWaveState::Waiting:
		TimeUntilNextWave -= DeltaTime;
		if (TimeUntilNextWave <= 0.f)
		{
			StartNextWave();
		}
		break;

	case EWaveState::Progress:
		if (EnemyToSpawn > 0)
		{
			SpawnTimer -= DeltaTime;
			if (SpawnTimer <= 0.f)
			{
				TSubclassOf<APawn> SpawnClass = (EnemyToSpawn % 2 == 0) ? MeleeEnemyC : RangedEnemyC;
				SpawnEnemy(SpawnClass);
				SpawnTimer = CurrSpawnIntev;
			}
		}

		if (EnemyToSpawn == 0 && LivingEnemy.Num() == 0)
		{
			EndCurrWave();
		}
		break;

	case EWaveState::Complete:
	case EWaveState::Fail:
		break;
	}
}

void UOAWaveManager::CheckWaveTrigger(float DeltaTime)
{
	CurrMapStayTime += DeltaTime;
	if (CurrMapStayTime >= StayDuration)
	{
		TriggerWave(EWaveTrigger::TimeOver);
	}
}

void UOAWaveManager::OnMapTrans()
{
	RefreshSpawn();
	ResetMapTimer();
	TriggerWave(EWaveTrigger::MapTransition);
}

void UOAWaveManager::TriggerWave(EWaveTrigger TriggerType)
{
	if (WaveStat == EWaveState::Waiting || WaveStat == EWaveState::Progress)
	{
		return;
	}

	LastTrigger = TriggerType;

	OnWaveTrigger.Broadcast();

	WaveStat = EWaveState::Waiting;
	TimeUntilNextWave = PrepareTime;
}

void UOAWaveManager::StartNextWave()
{
	StartWave(CurrWave + 1);
}

void UOAWaveManager::StartWave(int32 WaveNum)
{
	CurrWave = WaveNum;
	WaveStat = EWaveState::Progress;

	FWaveData WaveData = WaveDataGen(CurrWave);
	EnemyToSpawn = WaveData.TotalEnemy;
	RemainingEnemy = WaveData.TotalEnemy;
	CurrSpawnIntev = WaveData.SpawnIntev;
	SpawnTimer = 0.f;

	OnWaveStart.Broadcast(CurrWave);
}

void UOAWaveManager::EndCurrWave()
{
	WaveStat = EWaveState::Complete;

	OnWaveComplete.Broadcast(CurrWave);

	WaveStat = EWaveState::Idle;
	ResetMapTimer();
}

void UOAWaveManager::ResetMapTimer()
{
	CurrMapStayTime = 0.f;
}

void UOAWaveManager::RegisterEnemy(APawn* Enemy)
{
	if (Enemy && !LivingEnemy.Contains(Enemy))
	{
		LivingEnemy.Add(Enemy);
	}
}

void UOAWaveManager::UnregisterEnemy(APawn* Enemy)
{
	LivingEnemy.Remove(Enemy);
	RemainingEnemy = FMath::Max(0, RemainingEnemy - 1);
}

void UOAWaveManager::RefreshSpawn()
{
	ActiveSpawnPoint.Empty();

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SpawnPoint"), ActiveSpawnPoint);
}

FVector UOAWaveManager::GetRandomSpawnByType(EEnemyMoveType EMType) const
{
	TArray<AActor*> Filter;

	for (AActor* Point : ActiveSpawnPoint) {
		AOASpawnPoint* Spawn = Cast<AOASpawnPoint>(Point);
		if (Spawn && Spawn->GetSpawnType() == EMType) Filter.Add(Point);
	}

	if (Filter.Num() == 0) return GetRandomSpawnPoint();

	int32 idx = FMath::RandRange(0, Filter.Num() - 1);
	return Filter[idx]->GetActorLocation();
}

FWaveData UOAWaveManager::WaveDataGen_Implementation(int32 WaveNum)
{
	FWaveData D;
	D.WaveNum = WaveNum;

	D.TotalEnemy = 20 + (WaveNum * 2);
	D.MEnemy = FMath::CeilToInt(D.TotalEnemy * 0.5f);
	D.REnemy = FMath::CeilToInt(D.TotalEnemy * 0.5f);
	D.SpawnIntev = FMath::Max(0.5f, 1.0f - (WaveNum * 0.1f));

	return D;
}

FVector UOAWaveManager::GetRandomSpawnPoint() const
{
	if (ActiveSpawnPoint.Num() == 0) return FVector(1000.f, 0.f, 100.f);

	int32 idx = FMath::RandRange(0, ActiveSpawnPoint.Num() - 1);
	return ActiveSpawnPoint[idx]->GetActorLocation();
}

void UOAWaveManager::SpawnEnemy(TSubclassOf<APawn> EnemyC)
{
	if (!EnemyC || EnemyToSpawn <= 0)
	{
		return;
	}

	FVector SpawnLoc = GetRandomSpawnPoint();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APawn* Enemy = GetWorld()->SpawnActor<APawn>(EnemyC, SpawnLoc, FRotator::ZeroRotator, SpawnParams);

	if (Enemy)
	{
		RegisterEnemy(Enemy);
		EnemyToSpawn--;
	}
}

void UOAWaveManager::ResetWave()
{
	CurrWave = 0;
	WaveStat = EWaveState::Idle;
	TimeUntilNextWave = 0.f;
	CurrMapStayTime = 0.f;
	LivingEnemy.Empty();
	EnemyToSpawn = 0;
	RemainingEnemy = 0;
}