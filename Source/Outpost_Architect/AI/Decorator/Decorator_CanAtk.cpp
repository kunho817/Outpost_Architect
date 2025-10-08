// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/Decorator_CanAtk.h"
#include "AIController.h"
#include "Characters/OAEnemyBase.h"

UDecorator_CanAtk::UDecorator_CanAtk()
{
	NodeName = "Can Attack";
}

bool UDecorator_CanAtk::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMem) const
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return false;

	AOAEnemyBase* Enemy = Cast<AOAEnemyBase>(AICon->GetPawn());
	if (!Enemy) return false;

	return Enemy->CanAtk();
}