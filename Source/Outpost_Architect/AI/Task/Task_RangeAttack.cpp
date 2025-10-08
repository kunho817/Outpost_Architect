// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/Task_RangeAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/OAEnemyBase.h"
#include "Combat/OAProjectile.h"

UTask_RangeAttack::UTask_RangeAttack()
{
	NodeName = "Ranged Attack";

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UTask_RangeAttack, TargetActorKey), AActor::StaticClass());
}

EBTNodeResult::Type UTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMem)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	AOAEnemyBase* Enemy = Cast<AOAEnemyBase>(AICon->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	if (!Enemy->CanAtk()) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AActor* TargetA = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetA) return EBTNodeResult::Failed;

	FVector SpawnLoc = Enemy->GetActorLocation();

	FVector TargetLoc = TargetA->GetActorLocation();
	FVector Dir = (TargetLoc - SpawnLoc).GetSafeNormal();

	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = Enemy;
	SpawnParam.Instigator = Enemy;

	AOAProjectile* Proj = GetWorld()->SpawnActor<AOAProjectile>(Enemy->GetProjClass(), SpawnLoc, Dir.Rotation(), SpawnParam);

	if (Proj) {
		Proj->InitProjectile(Enemy->GetDmg(), Enemy, Dir);
		Enemy->DoAtk();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
