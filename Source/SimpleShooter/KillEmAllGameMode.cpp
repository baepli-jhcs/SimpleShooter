// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "ShooterAIController.h"
#include "ShooterPlayerController.h"
#include "StartLight.h"
#include "StartMover.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/AmbientSound.h"

void AKillEmAllGameMode::PawnKilled(APawn* Pawn)
{
	Super::PawnKilled(Pawn);

	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
	if (PlayerController)
	{
		return EndGame(false);
	}
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		if (AShooterAIController* AIController = Cast<AShooterAIController>(Controller))
		{
			if (!AIController->IsDead()) return;
		}
	};
	return EndGame(true);
}

void AKillEmAllGameMode::AddStartMover(UStartMover* StartMover)
{
	if (!StartMover || !StartMover->GetOwner()) return;
	StartMovers.Add(StartMover);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(nullptr, bIsWinner);
	};
}

void AKillEmAllGameMode::BeginGame()
{
	GetWorld()->SpawnActor(AmbientSoundClass);
	if (AShooterPlayerController* Controller = GetController())
	{
		Controller->StartHUD();
	}
	for (UStartMover* StartMover : StartMovers)
	{
		if (!StartMover) continue;
		StartMover->Move();
	}

	TArray<AActor*> StartLights;
	UGameplayStatics::GetAllActorsOfClass(this, StartLightClass, StartLights);
	for (AActor* StartLightActor : StartLights)
	{
		if (!StartLightActor) continue;
		if (AStartLight* StartLight = Cast<AStartLight>(StartLightActor))
		{
			StartLight->ChangeColor();
		}
	}
}

void AKillEmAllGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (AShooterPlayerController* PlayerController = GetController())
	{
		PlayerController->StartCountdown(GameStartDelay);
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AKillEmAllGameMode::DoCountdown, 1);
}

void AKillEmAllGameMode::DoCountdown()
{
	UE_LOG(LogTemp, Warning, TEXT("Here"));
	if (StartIterations >= (GameStartDelay - 1))
	{
		return BeginGame();
	}
	if (AShooterPlayerController* PlayerController = GetController())
	{
		PlayerController->DecrementCountdown();
	}
	StartIterations++;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AKillEmAllGameMode::DoCountdown, 1);
}

AShooterPlayerController* AKillEmAllGameMode::GetController()
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (!Controller) return nullptr;
	return Cast<AShooterPlayerController>(Controller);
}
