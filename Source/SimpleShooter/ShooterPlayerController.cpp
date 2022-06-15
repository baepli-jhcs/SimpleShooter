// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"

#include "Countdown.h"
#include "EndScreen.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	bPlayerWon = bIsWinner;
	HUD->RemoveFromViewport();
	GetWorldTimerManager().SetTimer(EndTimerHandle, this, &AShooterPlayerController::ShowEndScreen, DelayBeforeEnd);
}

void AShooterPlayerController::ShowEndScreen()
{
	UUserWidget* EndScreen = CreateWidget(this, EndScreenClass);
	if (!EndScreen) return;
	if (UEndScreen* End = Cast<UEndScreen>(EndScreen))
	{
		End->SetPlayerWon(bPlayerWon);
	}

	EndScreen->AddToViewport();
	ShowCursor(true);

	Pause();
}


void AShooterPlayerController::ShowCursor(bool ShouldShow)
{
	bShowMouseCursor = ShouldShow;
	bEnableClickEvents = ShouldShow;
	bEnableMouseOverEvents = ShouldShow;
}


void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AShooterPlayerController::StartCountdown(int32 Seconds)
{
	Countdown = CreateWidget(this, CountdownClass);
	if (!Countdown) return;
	if (UCountdown* CountdownWidget = Cast<UCountdown>(Countdown))
	{
		CountdownWidget->SetSeconds(Seconds);
	}

	Countdown->AddToViewport();
}


void AShooterPlayerController::StartHUD()
{
	if (Countdown)
	{
		DecrementCountdown(false);
		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle, this, &AShooterPlayerController::RemoveCountdown, 1);
	}
	HUD = CreateWidget(this, HUDClass);
	if (!HUD) return;

	HUD->AddToViewport();
}

void AShooterPlayerController::DecrementCountdown(bool bPlaySound)
{
	if (!Countdown) return;
	if (UCountdown* CountdownWidget = Cast<UCountdown>(Countdown))
	{
		CountdownWidget->Decrement();
	}
	if (bPlaySound && BeepSound && GetPawn())
	{
		UGameplayStatics::SpawnSoundAtLocation(this, BeepSound, GetPawn()->GetActorLocation(),
		                                       GetPawn()->GetActorRotation());
	}
}

void AShooterPlayerController::RemoveCountdown()
{
	if (!Countdown) return;
	Countdown->RemoveFromViewport();
}
