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

	TempHealth = Health;
	MaxHealth = Health;
}

// Called when the game starts or when spawned
void ADDCastle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADDCastle::ResetStats()
{

}

void ADDCastle::ApplyModifiers()
{

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

