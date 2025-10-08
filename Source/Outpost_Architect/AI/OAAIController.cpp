// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OAAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/OAEnemyBase.h"

const FName AOAAIController::TargetActorKey = TEXT("TargetActor");
const FName AOAAIController::SelfActorKey = TEXT("SelfActor");
const FName AOAAIController::AtkRangeKey = TEXT("AttackRange");
const FName AOAAIController::DetectRangeKey = TEXT("DetectRange");
const FName AOAAIController::EnemyTypeKey = TEXT("EnemyType");

AOAAIController::AOAAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AOAAIController::OnPossess(APawn* IPawn)
{
	Super::OnPossess(IPawn);

	if (!BT) return;

	if (BT->BlackboardAsset) {
		Blackboard->InitializeBlackboard(*BT->BlackboardAsset);

		if (AOAEnemyBase* Enemy = Cast<AOAEnemyBase>(IPawn)) {
			Blackboard->SetValueAsObject(SelfActorKey, Enemy);
			Blackboard->SetValueAsFloat(AtkRangeKey, Enemy->AtkRange);
			Blackboard->SetValueAsFloat(DetectRangeKey, Enemy->DetectRange);
			Blackboard->SetValueAsEnum(EnemyTypeKey, static_cast<uint8>(Enemy->ET));
		}

		RunBehaviorTree(BT);
	}
}

void AOAAIController::OnUnPossess()
{
	Super::OnUnPossess();

	if (BT) BrainComponent->StopLogic(TEXT("Unpossessed"));
}

void AOAAIController::SetTargetActor(AActor* Target)
{
	if (Blackboard) Blackboard->SetValueAsObject(TargetActorKey, Target);
}

AActor* AOAAIController::GetTargetActor() const
{
	if (Blackboard) return Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey));
	return nullptr;
}

