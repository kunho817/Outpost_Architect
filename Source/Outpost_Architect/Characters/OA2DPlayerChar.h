// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/OA2DCharacterBase.h"
#include "Inventory/OAInventoryComponent.h"
#include "OAEnum.h"
#include "OA2DPlayerChar.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class OUTPOST_ARCHITECT_API AOA2DPlayerChar : public AOA2DCharacterBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Val);
	void Interact();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UOAInventoryComponent* Inv = nullptr;

public:
	AOA2DPlayerChar();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* ViewCam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* MapContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	UOAInventoryComponent* GetInv() const { return Inv; }
};
