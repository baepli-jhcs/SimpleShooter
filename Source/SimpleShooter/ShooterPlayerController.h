// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

class UEndScreen;
/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;

	UFUNCTION(BlueprintCallable)
	void ShowCursor(bool ShouldShow);

	void StartHUD();

	void StartCountdown(int32 Seconds);

	void DecrementCountdown(bool bPlaySound = true);

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle EndTimerHandle;

	void ShowEndScreen();
	void RemoveCountdown();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> EndScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CountdownClass;

	UPROPERTY(EditAnywhere)
	float DelayBeforeEnd{1.5f};

	bool bPlayerWon;

	UPROPERTY()
	UUserWidget* HUD;

	UPROPERTY()
	UUserWidget* Countdown;

	UPROPERTY(EditAnywhere)
	USoundBase* BeepSound;
};
