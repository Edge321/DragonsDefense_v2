// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");

	RootComponent = Mesh;
	Collider->SetupAttachment(Mesh);

	Health = 10.0f;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//Prevents enemy from accelerating like crazy at the beginning
	FloatingPawnMovement->MaxSpeed = MovementSpeed;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector ActualMovement = FVector(DeltaTime * MovementSpeed * -1.0, 0, 0);
	AddMovementInput(ActualMovement);

	CheckDistance();
}
//Checks distance between the enemy and the castle
FVector AEnemy::CheckDistance()
{
	//TODO - implementing checking the distance
	return FVector();
}

void AEnemy::ApplyModifiers()
{
	//TODO - Apply modifiers to health, speed, shoot cooldown, and any others
}

void AEnemy::OnDeath()
{
	Destroy();
}