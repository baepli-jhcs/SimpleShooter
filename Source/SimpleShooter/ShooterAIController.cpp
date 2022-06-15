// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "ShooterCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AShooterAIController::AShooterAIController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn) return;
	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior);
		if (GetBlackboardComponent() && GetPawn())
			GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AShooterAIController::IsDead() const
{
	if (AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(GetPawn()))
	{
		return ShooterCharacter->IsDead();
	}
	return true;
}
