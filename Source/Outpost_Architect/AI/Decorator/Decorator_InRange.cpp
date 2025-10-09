// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/Decorator_InRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"

UDecorator_InRange::UDecorator_InRange()
{
	NodeName = "In Range";
}

bool UDecorator_InRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return false;

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return false;

	APawn* CPawn = AICon->GetPawn();
	if (!CPawn) return false;

	AActor* TargetA = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetA) return false;

	float Range = BB->GetValueAsFloat(RangeKey.SelectedKeyName);

	float Dist = FVector::Dist(CPawn->GetActorLocation(), TargetA->GetActorLocation());

	return Dist <= Range;
}