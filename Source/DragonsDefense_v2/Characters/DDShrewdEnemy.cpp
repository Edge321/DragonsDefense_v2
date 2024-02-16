// Fill out your copyright notice in the Description page of Project Settings.


#include "DDShrewdEnemy.h"

void ADDShrewdEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TimeSinceStart = GetWorld()->GetTimeSeconds();

	//TODO - optimize it so only one place is calculating this, cus there can be huge problems with lots of shrewd enemies on the screen
	float MovementX = DeltaTime * MovementSpeed * -1.0;
	float MovementY = DeltaTime * (Amplitude * (FMath::Sin(Frequency * TimeSinceStart)));

	FVector ActualMovement(MovementX, MovementY, 0);

	FloatingPawnMovement->AddInputVector(ActualMovement);

	CheckDistance();
}
