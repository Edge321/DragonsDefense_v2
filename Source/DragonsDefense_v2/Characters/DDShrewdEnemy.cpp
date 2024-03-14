// Fill out your copyright notice in the Description page of Project Settings.


#include "DDShrewdEnemy.h"

void ADDShrewdEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TimeSinceStart = GetWorld()->GetTimeSeconds();

	float MovementX = DeltaTime * MovementSpeed * -1.0;
	float MovementY = DeltaTime * (Amplitude * (FMath::Sin(Frequency * TimeSinceStart)));

	FVector ActualMovement(MovementX, MovementY, 0);

	FloatingPawnMovement->AddInputVector(ActualMovement);

	CheckDistance();
}
