// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCastle.h"
#include "Components/BoxComponent.h"
#include "../Characters/DDCastle.h"

// Sets default values
ADDCastle::ADDCastle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	RootComponent = Mesh;
	Collider->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ADDCastle::BeginPlay()
{
	Super::BeginPlay();

	TempHealth = Health;
	MaxHealth = Health;
}

void ADDCastle::UpdateHealth(const float HealthModifier)
{
	TempHealth += HealthModifier;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, 
		FString::Printf(TEXT("%s's Health: %f"), *GetName(), TempHealth));

	if (TempHealth <= 0)
	{
		OnDeath();
	}
}

const float ADDCastle::GetHealth()
{
	return TempHealth;
}

void ADDCastle::SetMaxHealth(const float MaxHealthModifier)
{
	MaxHealth = MaxHealthModifier;
}

const float ADDCastle::GetMaxHealth()
{
	return MaxHealth;
}

void ADDCastle::ResetStats()
{
	TempHealth = Health;
	MaxHealth = Health;
}

void ADDCastle::ApplyModifiers()
{
	//TODO - Do what the function says
}

void ADDCastle::OnDeath()
{
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->GameOver();
	}
	GameOverEventFunction();
}

void ADDCastle::GameOverEventFunction()
{
	ResetStats();
}

void ADDCastle::GameStartEventFunction()
{
	//TODO - Figure something out here. Might not need this function?
}