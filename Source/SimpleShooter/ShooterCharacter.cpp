// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "Gun.h"
#include "SimpleShooterGameModeBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!GunClass) return;

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	Health = MaxHealth;

	if (!Camera) return;
	Camera->SetFieldOfView(StandardFOV);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CurrentFOV = Camera->FieldOfView;
	float TargetFOV = ZoomedIn ? AimDownSightsFOV : StandardFOV;

	float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, CameraTransitionRate);

	Camera->SetFieldOfView(NewFOV);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("TimedLookUp"), this, &AShooterCharacter::TimedLookUp);
	PlayerInputComponent->BindAxis(TEXT("TimedLookRight"), this, &AShooterCharacter::TimedLookRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AShooterCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AShooterCharacter::StartSprinting);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AShooterCharacter::StopSprinting);
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &AShooterCharacter::ZoomIn);
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &AShooterCharacter::ZoomOut);
}


void AShooterCharacter::MoveForward(float Value)
{
	FVector Direction = GetActorForwardVector() * Value;

	AddMovementInput(Direction);
}

void AShooterCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AShooterCharacter::TimedLookUp(float Value)
{
	AddControllerPitchInput(Value * GetWorld()->GetDeltaSeconds() * RotationRate);
}

void AShooterCharacter::TimedLookRight(float Value)
{
	AddControllerYawInput(Value * GetWorld()->GetDeltaSeconds() * RotationRate);
}

void AShooterCharacter::Shoot()
{
	if (!Gun) return;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	Gun->PullTrigger();
}

void AShooterCharacter::StartSprinting()
{
	if (ZoomedIn) return;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * SprintMultiplier;
}

void AShooterCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                    AActor* DamageCauser)
{
	if (Health <= 0.f) return 0;
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	if (IsDead())
	{
		if (ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>()
		)
			GameMode->PawnKilled(this);
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (EventInstigator->GetCharacter())
		{
			if (AShooterCharacter* Killer = Cast<AShooterCharacter>(EventInstigator->GetCharacter()))
			{
				Killer->GainHealth(RegeneratedHealthAfterKill);
			}
		}
	}


	return DamageToApply;
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

void AShooterCharacter::ZoomIn()
{
	StopSprinting();
	ZoomedIn = true;
}

void AShooterCharacter::ZoomOut()
{
	ZoomedIn = false;
}

float AShooterCharacter::GetHealth() const
{
	return Health / MaxHealth;
}

void AShooterCharacter::GainHealth(const float Value)
{
	Health += Value;
	Health = FMath::Min(MaxHealth, Health);
}
