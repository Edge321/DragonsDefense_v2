// Fill out your copyright notice in the Description page of Project Settings.

#include "DDPlayer.h"
#include "../Projectile/DDProjectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
ADDPlayer::ADDPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatPawnMove");

	RootComponent = Mesh;

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ADDPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	ValidateProjectile();
}

// Called to bind functionality to input
void ADDPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADDPlayer::ValidateProjectile()
{
	if (Projectile == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Projectile not set on %s"), *GetName())
	}
}

void ADDPlayer::ResetStats()
{

}