// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OAAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/OAEnemyBase.h"

const FName AOAAIController::CurrentActorKey = TEXT("CurrentTarget");
const FName AOAAIController::CoreTargetKey = TEXT("CoreTarget");
const FName AOAAIController::PlayerTargetKey = TEXT("PlayerTarget");
const FName AOAAIController::ObstacleTargetKey = TEXT("ObstacleTarget");
const FName AOAAIController::SelfActorKey = TEXT("SelfActor");
const FName AOAAIController::AtkRangeKey = TEXT("AttackRange");
const FName AOAAIController::DetectRangeKey = TEXT("DetectionRange");
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
			Blackboard->SetValueAsFloat(AtkRangeKey, Enemy->GetAtkRange());
			Blackboard->SetValueAsFloat(DetectRangeKey, Enemy->GetDetectRange());
			Blackboard->SetValueAsEnum(EnemyTypeKey, static_cast<uint8>(Enemy->GetEnemyType()));
		}

		RunBehaviorTree(BT);
	}
}

void AOAAIController::OnUnPossess()
{
	Super::OnUnPossess();

	if (BT) BrainComponent->StopLogic(TEXT("Unpossessed"));
}