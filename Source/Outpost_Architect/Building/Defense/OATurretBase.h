// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/OABuildingBase.h"
#include "OAEnum.h"
#include "OATurretBase.generated.h"

/**
 * 
 */

class USphereComponent;
class AOAProjectile;

UENUM(BlueprintType)
enum class ETargetPriority : uint8
{
	Closest,
	Farthest,
	LowHP,
	HighHP
};


UCLASS(BlueprintType)
class OUTPOST_ARCHITECT_API AOATurretBase : public AOABuildingBase
{
	GENERATED_BODY()
	
public:
	AOATurretBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USphereComponent* DetectRange;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USceneComponent* TurretHead;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USceneComponent* ShotPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret|Settings")
	ETurretAttackType AtkType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret|Settings")
	ETargetPriority Prio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret|Settings")
	float DectectRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret|Settings")
	float RotateSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret|Combat")
	float AtkDmg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret|Combat")
	float AtkRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret|Combat")
	float AtkCool;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret|Combat", meta = (EditCondition = "AtkType == ETurretAttackType::Projectile"))
	TSubclassOf<AOAProjectile> ProjClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Targeting")
	AActor* CurrTarget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Targeting")
	TArray<AActor*> DetectEnemy;

	float PrevAtkTime;

	FTimerHandle SearchTimerHandler;

public:
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SearchTarget();
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SelectTarget();
	UFUNCTION(BlueprintCallable, Category = "Turret")
	bool IsValidTarget(AActor* Target);
	UFUNCTION(BlueprintCallable, Category = "Turret")
	AActor* GetCurrTarget() const { return CurrTarget; }

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void DoAtk();
	UFUNCTION(BlueprintCallable, Category = "Turret")
	bool CanAtk();
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void ShootProjectile();
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void ShootHitscan();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void RotateToTarget(float DeltaTime);
	UFUNCTION(BlueprintPure, Category = "Turret")
	bool IsAimToTarget();

protected:

	UFUNCTION()
	void OnDetectBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnDetectEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintNativeEvent, Category = "Turret")
	void OnTargetAcquire(AActor* Target);
	virtual void OnTargetAcquire_Implementation(AActor* Target);

	UFUNCTION(BlueprintNativeEvent, Category = "Turret")
	void OnTargetLost();
	virtual void OnTargetLost_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Turret")
	void OnFire();
	virtual void OnFire_Implementation();

};
