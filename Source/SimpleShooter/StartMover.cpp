// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMover.h"

#include "KillEmAllGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UStartMover::UStartMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UStartMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!ShouldMove) return;

	AActor* Owner = GetOwner();
	if (!Owner) return;

	const FVector CurrentLocation = Owner->GetActorLocation();
	if (CurrentLocation == TargetLocation) return;

	const FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);

	Owner->SetActorLocation(NewLocation);
}


// Called when the game starts
void UStartMover::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AActor const* Owner = GetOwner();
	if (!Owner) return;
	TargetLocation = Owner->GetActorLocation() + MoveOffset;

	AKillEmAllGameMode* GameMode = Cast<AKillEmAllGameMode>(UGameplayStatics::GetGameMode(this));
	if (!GameMode) return;
	GameMode->AddStartMover(this);
}


void UStartMover::Move()
{
	ShouldMove = true;
}
