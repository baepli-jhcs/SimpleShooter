// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndScreen.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UEndScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPlayerWon(bool DidPlayerWin);


private:
	UPROPERTY(VisibleAnywhere, Category = "Game", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bPlayerWon = false;
};
