// Fill out your copyright notice in the Description page of Project Settings.


#include "LivingActor.h"
#include "Components/BoxComponent.h"

// Sets default values
ALivingActor::ALivingActor()
{
	//Class that inherits is responsible for setting the root component
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

const float ALivingActor::GetHealth()
{
	return Health;
}

void ALivingActor::UpdateHealth(const float HealthModifier)
{
	Health += HealthModifier;

	//Temporary line until health bars in-game exist
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%s's Health: %f"), *GetName(), Health));

	if (Health <= 0)
	{
		OnDeath();
	}
}