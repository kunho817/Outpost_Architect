// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/Task_AttackObs.h"
#include "AI/OAAIController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/OAEnemyBase.h"
#include "Building/OABuildingBase.h"
#include "Interfaces/OnDamage.h"

UTask_AttackObs::UTask_AttackObs()
{
	NodeName = TEXT("Attack Obstacle");
	bNotifyTick = true;
}

EBTNodeResult::Type UTask_AttackObs::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if(!AICon) return EBTNodeResult::Failed;

	AOAEnemyBase* Enemy = Cast<AOAEnemyBase>(AICon->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if(!BB) return EBTNodeResult::Failed;

	AActor* Obs = Cast<AActor>(BB->GetValueAsObject(AOAAIController::ObstacleTargetKey));
	if(!Obs) return EBTNodeResult::Failed;

	AOABuildingBase* Building = Cast<AOABuildingBase>(Obs);
	if (!Building || !Building->IsOperation()) {
		BB->ClearValue(AOAAIController::ObstacleTargetKey);
		return EBTNodeResult::Succeeded;
	}

	float Dist = FVector::Dist(Enemy->GetActorLocation(), Obs->GetActorLocation());
	if(Dist > AttackRange) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UTask_AttackObs::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AOAEnemyBase* Enemy = Cast<AOAEnemyBase>(AICon->GetPawn());
	if (!Enemy) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AActor* Obs = Cast<AActor>(BB->GetValueAsObject(AOAAIController::ObstacleTargetKey));
	if (!Obs || !Obs->IsValidLowLevel()) {
		BB->ClearValue(AOAAIController::ObstacleTargetKey);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	AOABuildingBase* Building = Cast<AOABuildingBase>(Obs);
	if (!Building || !Building->IsOperation()) {
		BB->ClearValue(AOAAIController::ObstacleTargetKey);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	float Dist = FVector::Dist(Enemy->GetActorLocation(), Obs->GetActorLocation());
	if (Dist > AttackRange * 1.5f) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (Enemy->CanAtk()) {
		IOnDamage* DI = Cast<IOnDamage>(Building);
		if (DI) {
			FDamage DInfo;
			DInfo.Damage = Enemy->AtkDmg;
			DInfo.Instigator = Enemy;

			IOnDamage::Execute_TakeDamage(Building, DInfo);
			Enemy->PrevAtkTime = GetWorld()->GetTimeSeconds();
		}
	}
}
