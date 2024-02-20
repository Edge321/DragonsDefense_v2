// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Characters/LivingActor.h"
#include "DDPlaceable.generated.h"

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

	//Pure virtuals
	virtual const FVector GetActualMeshSize() const { return FVector(); };
	virtual const UStaticMeshComponent* GetMesh() const { return nullptr; };

	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	float EasyDamageMod = 1.2f;
	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	float HardDamageMod = 0.9f;

public:

	FOnPlaceableDeath OnPlaceableDeath;
};
