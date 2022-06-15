// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "ShooterCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::PullTrigger()
{
	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	}
	if (MuzzleSound)
	{
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
	}


	FVector ShotDirection;
	FHitResult HitResult;

	bool bHit = GunTrace(HitResult, ShotDirection);
	if (!bHit) return;
	AController* OwnerController = GetOwnerController();
	if (!OwnerController) return;

	if (ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect, HitResult.ImpactPoint, ShotDirection.Rotation());
	}

	if (ImpactSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, HitResult.ImpactPoint, ShotDirection.Rotation());
	}

	if (AActor* HitActor = HitResult.GetActor())
	{
		FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
		HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
	}
}

bool AGun::GunTrace(FHitResult& HitResult, FVector& ShotDirection)
{
	const AController* OwnerController = GetOwnerController();
	if (!OwnerController) return false;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	const FVector End = Location + Rotation.Vector() * Range;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	ShotDirection = -Rotation.Vector();

	return GetWorld()->LineTraceSingleByChannel(HitResult, Location, End, ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return false;
	AController* OwnerController = OwnerPawn->GetController();
	return OwnerController;
}
