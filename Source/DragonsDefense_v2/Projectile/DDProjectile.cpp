// Fill out your copyright notice in the Description page of Project Settings.


#include "DDProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ADDProjectile::ADDProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Collider = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	RootComponent = Mesh;
	Collider->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ADDProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}
