// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ETeam : uint8
{
	Player UMETA(DisplayName = "Player"),
	Enemy UMETA(DisplayName = "Enemy"),
	Neutral UMETA(DisplayName = "Neutral"),
};

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Melee UMETA(DisplayName = "Melee"),
	Ranged UMETA(DisplayName = "Range")
};

UENUM(BlueprintType)
enum class EEnemyMoveType : uint8
{
	Normal UMETA(DisplayName = "Ground Enemy"),
	Fly UMETA(DisplayName = "Fly Enemy")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	Attack UMETA(DisplayName = "Attack"),
	BackMove UMETA(DisplayName = "BackMove"),
	Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EBuildingType : uint8
{
	Core UMETA(DisplayName = "Core"),
	Defense UMETA(DisplayName = "Defense"),
	Production UMETA(DisplayName = "Production"),
	Automation UMETA(DisplayName = "Automation")
};

UENUM(BlueprintType)
enum class ETurretAttackType : uint8
{
	Projectile UMETA(DisplayName = "Projectile"),
	Hitscan UMETA(DisplayName = "Hitscan")
};

UENUM(BlueprintType)
enum class EWaveTrigger : uint8
{
	MapTransition UMETA(DisplayName = "Map Transition"),
	TimeOver UMETA(DisplayName = "TimeOver")
};

UENUM(BlueprintType)
enum class EInventoryType : uint8
{
	Player UMETA(DisplayName = "Player Inventory"),
	Storage UMETA(DisplayName = "Storage Inventory"),
	Production UMETA(DisplayName = "Production Inventory")
};

class OUTPOST_ARCHITECT_API OAEnum
{
public:
	OAEnum();
	~OAEnum();
};
