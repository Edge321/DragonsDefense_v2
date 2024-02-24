// Fill out your copyright notice in the Description page of Project Settings.


#include "DDPlaceable.h"

// Sets default values
ADDPlaceable::ADDPlaceable()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Collider->SetCollisionObjectType(ECC_PlaceableChannel);
}

// Called when the game starts or when spawned
void ADDPlaceable::BeginPlay()
{
	Super::BeginPlay();
}

void ADDPlaceable::OnDeath()
{
	OnPlaceableDeath.ExecuteIfBound(this);
}
