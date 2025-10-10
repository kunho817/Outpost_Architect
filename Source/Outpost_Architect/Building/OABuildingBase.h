// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/OnDamage.h"
#include "Interfaces/Buildable.h"
#include "OAEnum.h"
#include "OAStruct.h"
#include "OABuildingBase.generated.h"

class UPaperSpriteComponent;
class UBoxComponent;

UENUM(BlueprintType)
enum class EBuildingState : uint8
{
	Construct,
	Enabled,
	Disabled,
	Destroyed
};

UCLASS()
class OUTPOST_ARCHITECT_API AOABuildingBase : public AActor, public IOnDamage, public IBuildable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOABuildingBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UBoxComponent* ColComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UPaperSpriteComponent* SpriteComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Info")
	EBuildingType BType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Info")
	FText BName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Info")
	FText BDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Info")
	EBuildingState BState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Info")
	FVector BuildingSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Info")
	ETeam Team = ETeam::Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Stat")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Stat")
	float CurrHealth;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Construct")
	FBuildCost BCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Construct")
	float ConstTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building|Construct")
	float ConstProg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Power")
	float PowerCons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building|Power")
	float PowerProd;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building|Power")
	bool GenPower;

	bool IsGhost = false;

public:
	virtual void TakeDamage_Implementation(const FDamage& DInfo) override;
	virtual float GetCurrHealth_Implementation() const override { return CurrHealth; }
	virtual float GetMaxHealth_Implementation() const override { return MaxHealth; }
	virtual bool Alive_Implementation() const override { return CurrHealth > 0; }
	virtual void Die_Implementation() override;

	virtual EBuildingType GetBType_Implementation() const override { return BType; }
	virtual FBuildCost GetBCost_Implementation() const override { return BCost; }
	virtual bool CanBuild_Implementation(const FVector& Loc) const override;
	virtual void ConstructBuilding_Implementation(const FVector& Loc) override;
	virtual void DestoryBuliding_Implementation() override;

	UFUNCTION(BlueprintPure, Category = "Building")
	FVector GetBuildingSize() const { return BuildingSize; }
	UFUNCTION(BlueprintCallable, Category = "Building")
	void SetBState(EBuildingState NState);
	UFUNCTION(BlueprintPure, Category = "Building")
	EBuildingState GetBState() const { return BState; }
	UFUNCTION(BlueprintPure, Category = "Building")
	bool IsOperation() const { return BState != EBuildingState::Destroyed; }
	UFUNCTION(BlueprintPure, Category = "Building")
	ETeam GetTeam() const { return Team; }


	UFUNCTION(BlueprintCallable, Category = "Building")
	void StartConstruct();
	UFUNCTION(BlueprintCallable, Category = "Building")
	void UpdateConstruct(float DeltaTime);
	UFUNCTION(BlueprintCallable, Category = "Building")
	void CompleteConstruct();

	UFUNCTION(BlueprintCallable, Category = "Building")
	void SetPowerState(bool Powered);
	UFUNCTION(BlueprintPure, Category = "Building")
	bool HasGenPower() const { return GenPower; }

	UFUNCTION(BlueprintCallable, Category = "Ghost")
	void SetGhostState(bool NewState);
	UFUNCTION(BlueprintPure, Category = "Ghost")
	bool GetGhostState() const { return IsGhost; }

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Building")
	void OnBuildingConstruct();
	virtual void OnBuildingConstruct_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Building")
	void OnBuildingDestroy();
	virtual void OnBuildingDestroy_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Building")
	void OnPowerStateChange(bool Powered);
	virtual void OnPowerStateChange_Implementation(bool Powered);
};
