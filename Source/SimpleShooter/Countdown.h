// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Countdown.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UCountdown : public UUserWidget
{
	GENERATED_BODY()

public:
	void Decrement();
	void SetSeconds(int32 NewSeconds);

private:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 Seconds;
};
