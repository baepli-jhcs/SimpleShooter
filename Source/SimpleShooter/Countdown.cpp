// Fill out your copyright notice in the Description page of Project Settings.


#include "Countdown.h"

void UCountdown::SetSeconds(int32 NewSeconds)
{
	Seconds = NewSeconds;
}


void UCountdown::Decrement()
{
	Seconds--;
}
