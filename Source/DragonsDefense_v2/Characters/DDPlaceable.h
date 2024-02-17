// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Characters/LivingActor.h"
#include "DDPlaceable.generated.h"

//TODO - Make placeables implement this!
#define ECC_PlaceableChannel ECC_GameTraceChannel2

DECLARE_DELEGATE_OneParam(FOnPlaceableDeath, ADDPlaceable*)

/**
 * 
 */
UCLASS(Abstract)
class DRAGONSDEFENSE_V2_API ADDPlaceable : public ALivingActor
{
	GENERATED_BODY()
	

public:
	ADDPlaceable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnDeath() override;

	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	float EasyDamageMod = 1.2f;
	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	float HardDamageMod = 0.9f;

public:

	FOnPlaceableDeath OnPlaceableDeath;
};
