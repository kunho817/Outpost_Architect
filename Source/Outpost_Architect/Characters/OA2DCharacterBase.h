// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Interfaces/OnDamage.h"
#include "OAStruct.h"
#include "OAEnum.h"
#include "OA2DCharacterBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class OUTPOST_ARCHITECT_API AOA2DCharacterBase : public APaperCharacter, public IOnDamage
{
	GENERATED_BODY()
	
public:
	AOA2DCharacterBase();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.f;
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float CurrHealth = 100.f;

	virtual void TakeDamage_Implementation(const FDamage& DInfo) override;
	virtual float GetCurrHealth_Implementation() const override;
	virtual float GetMaxHealth_Implementation() const override;
	virtual bool Alive_Implementation() const override;
	virtual void Die_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float Heal);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float JumpH = 500.f;

	UFUNCTION(BlueprintCallable, Category = "Sprite")
	void UpdateSpriteDir(float Dir);

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void OnDamageReceive(float Dmg);
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void Death();

	UFUNCTION(BlueprintPure, Category = "Team")
	ETeam GetTeam() const { return Team; }
	UFUNCTION(BlueprintCallable, Category = "Team")
	void SetTeam(ETeam NTeam) { Team = NTeam; }

protected:
	virtual void BeginPlay() override;
	virtual void DeathFinish();

	float LastDir = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Team")
	ETeam Team = ETeam::Player;
};
