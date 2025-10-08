// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/OA2DPlayerChar.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

AOA2DPlayerChar::AOA2DPlayerChar()
{
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = 1000.f;
	CameraArm->SocketOffset = FVector(0.f, 0.f, 0.f);
	CameraArm->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	CameraArm->bDoCollisionTest = false;

	ViewCam = CreateDefaultSubobject<UCameraComponent>(TEXT("View Camera"));
	ViewCam->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	ViewCam->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AOA2DPlayerChar::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* EIS = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer())) EIS->AddMappingContext(MapContext, 0);
	}
}

void AOA2DPlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOA2DPlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOA2DPlayerChar::Move);
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &AOA2DPlayerChar::Jump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &AOA2DPlayerChar::StopJumping);
		EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &AOA2DPlayerChar::Interact);
	}
}

void AOA2DPlayerChar::Move(const FInputActionValue& Val)
{
	float MoveValue = Val.Get<float>();

	if (Controller && MoveValue != 0.f) {
		AddMovementInput(FVector(1.f, 0.f, 0.f), MoveValue);
		UpdateSpriteDir(MoveValue);
	}
}

void AOA2DPlayerChar::Interact()
{

}