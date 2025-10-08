// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OAEnum.h"
#include "OAWaveManager.generated.h"

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	Idle UMETA(DisplayName = "Before Wave"),
	Waiting UMETA(DisplayName = "Prepare Time"),
	Progress UMETA(DisplayName = "In Progress"),
	Complete UMETA(DisplayName = "Complete"),
	Fail UMETA(DisplayName = "Failed")
};

USTRUCT(BlueprintType)
struct FWaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WaveNum = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalEnemy = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MEnemy = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 REnemy = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnIntev = 1.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveStarted, int32, WaveNum);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveCompleted, int32, WaveNum);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveTrigger);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OUTPOST_ARCHITECT_API UOAWaveManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOAWaveManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave Settings")
	float StayDuration = 120.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wave Settings")
	float PrepareTime = 10.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Wave State")
	int32 CurrWave = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Wave State")
	float TimeUntilNextWave = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Wave State")
	float CurrMapStayTime = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Wave State")
	EWaveState WaveStat = EWaveState::Waiting;
	UPROPERTY(BlueprintReadOnly, Category = "Wave State")
	int32 RemainingEnemy = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Wave State")
	int32 EnemyToSpawn = 0;

	UFUNCTION(BlueprintCallable, Category = "Wave Control")
	void OnMapTrans();
	UFUNCTION(BlueprintCallable, Category = "Wave Control")
	void TriggerWave(EWaveTrigger TriggerType);
	UFUNCTION(BlueprintCallable, Category = "Wave Control")
	void StartNextWave();
	UFUNCTION(BlueprintCallable, Category = "Wave Control")
	void StartWave(int32 WaveNum);
	UFUNCTION(BlueprintCallable, Category = "Wave Control")
	void EndCurrWave();
	UFUNCTION(BlueprintCallable, Category = "Wave Control")
	void ResetWave();
	UFUNCTION(BlueprintCallable, Category = "Wave Control")
	void ResetMapTimer();

	UFUNCTION(BlueprintCallable, Category = "Enemy Management")
	void RegisterEnemy(APawn* Enemy);
	UFUNCTION(BlueprintCallable, Category = "Enemy Management")
	void UnregisterEnemy(APawn* Enemy);
	UFUNCTION(BlueprintCallable, Category = "Enemy Management")
	int32 GetLivingEnemyCnt() const { return LivingEnemy.Num(); }
	UFUNCTION(BlueprintCallable, Category = "Enemy Management")
	TArray<APawn*> GetLivingEnemy() const { return LivingEnemy; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<APawn> MeleeEnemyC;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<APawn> RangedEnemyC;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWaveStarted OnWaveStart;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWaveCompleted OnWaveComplete;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWaveTrigger OnWaveTrigger;

protected:

	UPROPERTY()
	TArray<APawn*> LivingEnemy;

	float SpawnTimer = 0.f;
	float CurrSpawnIntev = 1.0f;

	UPROPERTY()
	TArray<AActor*> ActiveSpawnPoint;

	void RefreshSpawn();
	FVector GetRandomSpawnByType(EEnemyMoveType EMType) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Wave Generation")
	FWaveData WaveDataGen(int32 WaveNum);
	virtual FWaveData WaveDataGen_Implementation(int32 WaveNum);

	FVector GetRandomSpawnPoint() const;
	void SpawnEnemy(TSubclassOf<APawn> EnemyC);

	EWaveTrigger LastTrigger = EWaveTrigger::MapTransition;
	void CheckWaveTrigger(float DeltaTime);
};
