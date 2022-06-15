// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AGun;
UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	void Shoot();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealth() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TimedLookUp(float Value);
	void TimedLookRight(float Value);
	void StartSprinting();
	void StopSprinting();
	void ZoomIn();
	void ZoomOut();
	void GainHealth(float Value);

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationRate{60.f};

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintMultiplier{1.7f};

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AGun> GunClass;


	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MaxHealth{100.f};

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float AimDownSightsFOV{45.f};

	UPROPERTY(EditAnywhere, Category = "Camera")
	float StandardFOV{80.f};

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraTransitionRate{5.f};

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed{320.f};

	UPROPERTY(EditAnywhere, Category = "Combat")
	float RegeneratedHealthAfterKill{30.f};

	UPROPERTY()
	AGun* Gun;

	bool ZoomedIn{false};

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;
};
