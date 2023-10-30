// Fill out your copyright notice in the Description page of Project Settings.


#include "LivingActor.h"
#include "Components/BoxComponent.h"

// Sets default values
ALivingActor::ALivingActor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
}

// Called when the game starts or when spawned
void ALivingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALivingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ALivingActor::GetHealth()
{
	return Health;
}

void ALivingActor::SetHealth(float HealthModifier)
{
	Health += HealthModifier;

	if (Health <= 0) 
	{
		OnDeath();
	}
}