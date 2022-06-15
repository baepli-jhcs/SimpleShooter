// Fill out your copyright notice in the Description page of Project Settings.


#include "StartLight.h"

#include "Components/PointLightComponent.h"

// Sets default values
AStartLight::AStartLight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	SetRootComponent(PointLight);
}

// Called when the game starts or when spawned
void AStartLight::BeginPlay()
{
	Super::BeginPlay();
	if (!PointLight) return;
	PointLight->SetLightColor(StartColor);
}

// Called every frame
void AStartLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStartLight::ChangeColor()
{
	PointLight->SetLightColor(EndColor);
}
