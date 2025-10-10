// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OAEnum.h"
#include "OAProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UPaperSpriteComponent;

UCLASS()
class OUTPOST_ARCHITECT_API AOAProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOAProjectile();

	void InitProjectile(float NDmg, AActor* IInstigator, FVector Dir);
	void SetOwnerTeam(ETeam InTeam) { OwnerTeam = InTeam; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USphereComponent* ColComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UProjectileMovementComponent* ProjMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UPaperSpriteComponent* SpriteComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Stat")
	float Dmg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Stat")
	float ProjSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Stat")
	float MaxLifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Stat")
	bool DestroyOnHit;

	UPROPERTY()
	AActor* ProjInstigator;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Team")
	ETeam OwnerTeam;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherA, UPrimitiveComponent* OtherComp, FVector NormalImpusle, const FHitResult& hit);

	void ApplyDamage(AActor* Target);

	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void OnProjDestroy();
	virtual void OnProjDestroy_Implementation();

	FTimerHandle LifeTimerHandler;
	void EndLifeTime();

	bool IsFriendly(AActor* Target) const;

};
