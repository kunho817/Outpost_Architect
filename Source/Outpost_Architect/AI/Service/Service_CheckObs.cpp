// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/Service_CheckObs.h"
#include "AI/OAAIController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/OAEnemyBase.h"
#include "Building/OABuildingBase.h"
#include "DrawDebugHelpers.h"

UService_CheckObs::UService_CheckObs()
{
	NodeName = TEXT("Check Obstacle");
	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UService_CheckObs::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return;

	AOAEnemyBase* Enemy = Cast<AOAEnemyBase>(AICon->GetPawn());
	if (!Enemy) return;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;

	AActor* CurrTarget = Cast<AActor>(BB->GetValueAsObject(AOAAIController::CurrentActorKey));
	if (!CurrTarget) {
		BB->ClearValue(AOAAIController::ObstacleTargetKey);
		return;
	}

	FVector StartLoc = Enemy->GetActorLocation();
	FVector TargetLoc = CurrTarget->GetActorLocation();
	FVector Dir = (TargetLoc - StartLoc).GetSafeNormal();
	float Dist = FVector::Dist(StartLoc, TargetLoc);

	float TraceDist = OnlyCheckCloserThanTarget ? FMath::Min(CheckDist, Dist) : CheckDist;

	FVector EndLoc = StartLoc + (Dir * TraceDist);

	FHitResult HitRes;
	FCollisionQueryParams QParam;
	QParam.AddIgnoredActor(Enemy);

	bool IsHit = GetWorld()->LineTraceSingleByChannel(HitRes, StartLoc, EndLoc, ECC_GameTraceChannel5, QParam);

#if WITH_EDITOR
	if (IsHit) {
		DrawDebugLine(GetWorld(), StartLoc, HitRes.Location, FColor::Red, false, 0.5f, 0, 2.f);
		DrawDebugSphere(GetWorld(), HitRes.Location, 30.f, 12, FColor::Red, false, 0.5f);
	}
	else {
		DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Green, false, 0.5f, 0, 2.f);
	}
#endif

	if (IsHit && HitRes.GetActor()) {
		AOABuildingBase* Building = Cast<AOABuildingBase>(HitRes.GetActor());
		if (Building && Building != BB->GetValueAsObject(AOAAIController::CoreTargetKey) && Building->IsOperation()) {
			BB->SetValueAsObject(AOAAIController::ObstacleTargetKey, Building);
			return;
		}
	}

	BB->ClearValue(AOAAIController::ObstacleTargetKey);
}
