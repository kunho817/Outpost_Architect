// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "OAResourceNode.generated.h"

class UPaperSpriteComponent;
class UBoxComponent;

UCLASS()
class OUTPOST_ARCHITECT_API AOAResourceNode : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOAResourceNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UPaperSpriteComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UBoxComponent* ColComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	FName ItemID = "Iron";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 TotalAmount = 1000;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource")
	int32 RemainAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 MiningAmount = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	float MineCool = 1.0f;

	float PrevMiningTime = 0.f;

public:	
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual bool CanInteract_Implementation(AActor* Interactor) const override;
	virtual FText GetInteractionText_Implementation() const override;

	UFUNCTION(BlueprintPure, Category = "Resource")
	bool IsDepleted() const { return RemainAmount <= 0; }
	UFUNCTION(BlueprintPure, Category = "Resoruce")
	float GetDepletionPer() const;
	UFUNCTION(BlueprintPure, Category = "Resoruce")
	int32 GetRemain() const { return RemainAmount; }
	UFUNCTION(BlueprintPure, Category = "Resoruce")
	FName GetItemID() const { return ItemID; }

	UFUNCTION(BlueprintNativeEvent, Category = "Resource")
	void OnNodeDepleted();
	virtual void OnNodeDepleted_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "Resource")
	void OnMined(AActor* Miner, int32 Amount);
	virtual void OnMined_Implementation(AActor* Miner, int32 Amount);

protected:
	int32 Mine(int32 Request);

};
