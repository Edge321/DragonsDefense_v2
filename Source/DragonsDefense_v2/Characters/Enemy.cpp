// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "../Projectile/DDProjectile.h"
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

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
	FloatingPawnMovement->AddInputVector(ActualMovement);

	CheckDistance();
}
//Checks distance between the enemy and the castle
void AEnemy::CheckDistance()
{
	FVector SelfLocation = GetActorLocation();
	FVector CastleLocation = Castle->GetActorLocation();
	FVector2D SelfLocation2D = FVector2D(SelfLocation.X, SelfLocation.Y);
	FVector2D CastleLocation2D = FVector2D(CastleLocation.X, CastleLocation.Y);
	
	float Distance = FVector2D::Distance(SelfLocation2D, CastleLocation2D);

	UE_LOG(LogTemp, Log, TEXT("Distance: %f"), Distance)
	if (DistanceToAttack < Distance) {
		Shoot();
	}
	//TODO - implementing checking the distance
}

void AEnemy::ApplyModifiers()
{
	//TODO - Apply modifiers to health, speed, shoot cooldown, and any others
}

void AEnemy::OnDeath()
{
	Destroy();
}

void AEnemy::Shoot()
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Shoot Now!"));
	}
	FloatingPawnMovement->MaxSpeed = 0;
}