// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Characters/Enemy.h"
#include "DDShrewdEnemy.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API ADDShrewdEnemy : public AEnemy
{
	GENERATED_BODY()
	
public:

	void Tick(float DeltaTime) override;

protected:
	
	//How wide the zig-zag pattern is
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Amplitude = 500.0f;
	//How fast they are zig-zagging
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Frequency = 1.0f;

};
