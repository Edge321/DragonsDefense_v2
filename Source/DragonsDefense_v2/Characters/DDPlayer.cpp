// Fill out your copyright notice in the Description page of Project Settings.

#include "DDPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "../Projectile/DDProjectile.h"

// Sets default values
ADDPlayer::ADDPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatPawnMove");

	RootComponent = Mesh;

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ADDPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void ADDPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
