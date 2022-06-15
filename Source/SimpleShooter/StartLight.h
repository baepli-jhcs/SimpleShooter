// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StartLight.generated.h"

class UPointLightComponent;
UCLASS()
class SIMPLESHOOTER_API AStartLight : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStartLight();
	void ChangeColor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Light")
	FColor StartColor{
		255, 0, 0
	};

	UPROPERTY(EditAnywhere, Category = "Light")
	FColor EndColor{
		0, 255, 0
	};

	UPROPERTY(EditAnywhere)
	UPointLightComponent* PointLight;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
