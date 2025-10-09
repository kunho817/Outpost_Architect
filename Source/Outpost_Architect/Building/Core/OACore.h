// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/OABuildingBase.h"
#include "OACore.generated.h"

/**
 * 
 */
UCLASS()
class OUTPOST_ARCHITECT_API AOACore : public AOABuildingBase
{
	GENERATED_BODY()
	
public:
	AOACore();

protected:
	virtual void BeginPlay() override;

	virtual void Die_Implementation() override;

private:
	UPROPERTY();
	class AOAGameMode* GM;
};
