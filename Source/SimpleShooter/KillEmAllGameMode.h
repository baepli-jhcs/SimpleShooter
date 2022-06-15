// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "KillEmAllGameMode.generated.h"

class AShooterPlayerController;
class AAmbientSound;
class AStartLight;
class UStartMover;
/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AKillEmAllGameMode : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn* Pawn) override;
	void AddStartMover(UStartMover* StartMover);

protected:
	virtual void BeginPlay() override;

private:
	void EndGame(bool bIsPlayerWinner);

	TArray<UStartMover*> StartMovers;

	void BeginGame();
	void DoCountdown();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 GameStartDelay{5};

	int32 StartIterations{0};

	UPROPERTY(EditAnywhere)
	TSubclassOf<AAmbientSound> AmbientSoundClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AStartLight> StartLightClass;

	AShooterPlayerController* GetController();
};
