// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/Service_UpdateTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/OAEnemyBase.h"

UService_UpdateTarget::UService_UpdateTarget()
{
	NodeName = "Update Target";
	Interval = 0.5f;
	RandomDeviation = 0.f;
}

void UService_UpdateTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMem, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMem, DeltaSeconds);

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return;

	AOAEnemyBase* Enemy = Cast<AOAEnemyBase>(AICon->GetPawn());
	if (!Enemy) return;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;

	AActor* CurrTarget = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if(!CurrTarget || !CurrTarget->IsValidLowLevel()){
		AActor* NT = Enemy->FindTarget();
		if (NT) {
			BB->SetValueAsObject(TargetActorKey.SelectedKeyName, NT);
			Enemy->SetTarget(NT);
		}
	}
}