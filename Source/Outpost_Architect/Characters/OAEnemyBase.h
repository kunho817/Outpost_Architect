// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/OA2DCharacterBase.h"
#include "OAEnum.h"
#include "OAEnemyBase.generated.h"

class UOAWaveManager;
class UBehaviorTree;
class AOAProjectile;

UCLASS(Abstract)
class OUTPOST_ARCHITECT_API AOAEnemyBase : public AOA2DCharacterBase
{
	GENERATED_BODY()
	
public:
	AOAEnemyBase();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EEnemyType ET = EEnemyType::Melee;
	UPROPERTY(BlueprintReadWrite, Category = "AI")
	EEnemyState ES = EEnemyState::Idle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float DetectRange = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AtkRange = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AtkDmg = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AtkCool = 1.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float PrevAtkTime = 0.f;
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void DoAtk(AActor* Target);
	virtual void DoAtk();
	UFUNCTION(BlueprintPure, Category = "Combat")
	bool CanAtk() const;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Ranged")
	TSubclassOf<class AOAProjectile> ProjClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EEnemyMoveType MoveType = EEnemyMoveType::Normal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Fly")
	float FlyingHeight = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|BackMove")
	float BackMoveDist = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|BackMove")
	float BackMoveDur = 1.0f;

	UFUNCTION(BlueprintPure, Category = "Enemy")
	float GetAtkRange() const { return AtkRange; }
	UFUNCTION(BlueprintPure, Category = "Enemy")
	float GetDetectRange() const { return DetectRange; }
	UFUNCTION(BlueprintPure, Category = "Enemy")
	EEnemyType GetEnemyType() const { return ET; }
	UFUNCTION(BlueprintPure, Category = "Enemy")
	EEnemyMoveType GetMoveType() const { return MoveType; }
	UFUNCTION(BlueprintPure, Category = "Combat")
	TSubclassOf<AOAProjectile> GetProjClass() const { return ProjClass; }
	UFUNCTION(BlueprintPure, Category = "Combat")
	float GetDmg() const { return AtkDmg; }

	virtual void Die_Implementation() override;

protected:
	virtual void BeginPlay() override;
	virtual void DeathFinish() override;

	UOAWaveManager* WaveMan = nullptr;
};
