// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"

#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (!Owner) return EBTNodeResult::Failed;
	AShooterCharacter* Character = Cast<AShooterCharacter>(Owner);
	if (!Character) return EBTNodeResult::Failed;
	Character->Shoot();

	return EBTNodeResult::Succeeded;
}
