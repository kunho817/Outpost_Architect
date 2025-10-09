// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/Service_EvaluateTarget.h"
#include "AI/OAAIController.h"
#include "Characters/OAEnemyBase.h"
#include "Building/Core/OACore.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UService_EvaluateTarget::UService_EvaluateTarget()
{
	NodeName = TEXT("Evaluate Target Priority");
	bNotifyBecomeRelevant = true;
	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UService_EvaluateTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AOAAIController* AICon = Cast<AOAAIController>(OwnerComp.GetAIOwner());
	if (!AICon) return;

	AOAEnemyBase* Enemy = Cast<AOAEnemyBase>(AICon->GetPawn());
	if (!Enemy) return;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;

	AActor* Core = Cast<AActor>(BB->GetValueAsObject(AOAAIController::CoreTargetKey));
	AActor* Player = Cast<AActor>(BB->GetValueAsObject(AOAAIController::PlayerTargetKey));
	AActor* CurrTarget = EvaluateCurrTarget(Enemy, Core, Player);
	AActor* PrevTarget = Cast<AActor>(BB->GetValueAsObject(AOAAIController::CurrentActorKey));

	if (CurrTarget != PrevTarget) BB->SetValueAsObject(AOAAIController::CurrentActorKey, CurrTarget);
}

void UService_EvaluateTarget::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;

	UWorld* World = GetWorld();
	if (!World) return;

	AActor* Core = FindCoreTarget(World);
	if (Core) BB->SetValueAsObject(AOAAIController::CoreTargetKey, Core);

	AActor* Player = FindPlayerTarget(World);
	if (Player) BB->SetValueAsObject(AOAAIController::PlayerTargetKey, Player);


	AOAAIController* AICon = Cast<AOAAIController>(OwnerComp.GetAIOwner());
	if (AICon) {
		AOAEnemyBase* Enemy = Cast<AOAEnemyBase>(AICon->GetPawn());
		if (Enemy) {
			AActor* InitTarget = EvaluateCurrTarget(Enemy, Core, Player);
			BB->SetValueAsObject(AOAAIController::CurrentActorKey, InitTarget);
		}
	}
}

AActor* UService_EvaluateTarget::FindCoreTarget(UWorld* World) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, AOACore::StaticClass(), FoundActors);
	return (FoundActors.Num() > 0) ? FoundActors[0] : nullptr;
}

AActor* UService_EvaluateTarget::FindPlayerTarget(UWorld* World) const
{
	return UGameplayStatics::GetPlayerPawn(World, 0);
}

AActor* UService_EvaluateTarget::EvaluateCurrTarget(AOAEnemyBase* Enemy, AActor* Core, AActor* Player) const
{
	if (!Enemy) return nullptr;

	if (Player && IsPlayerInDetectRange(Enemy, Player)) {
		return Player;
	}else if (Core) {
		return Core;
	}
	else if (Player) {
		return Player;
	}

	return nullptr;
}

bool UService_EvaluateTarget::IsPlayerInDetectRange(AOAEnemyBase* Enemy, AActor* Player) const
{
	if (!Enemy || !Player) return false;

	FVector EnemyLoc = Enemy->GetActorLocation();
	FVector PlayerLoc = Player->GetActorLocation();
	float DetectRange = Enemy->GetDetectRange();

	if (Enemy->GetMoveType() == EEnemyMoveType::Normal) {
		float Dist = FMath::Abs(EnemyLoc.X - PlayerLoc.X);
		return Dist <= DetectRange;
	}
	else {
		FVector EnemyPos = FVector(EnemyLoc.X, 0.f, EnemyLoc.Z);
		FVector PlayerPos = FVector(PlayerLoc.X, 0.f, PlayerLoc.Z);
		float Dist = FVector::Dist(EnemyPos, PlayerPos);
		return Dist <= DetectRange;
	}
}
