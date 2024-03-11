// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Characters/LivingActor.h"
#include "DDPlaceable.generated.h"

#define Stencil_HighlightDepth 3

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

	UFUNCTION(BlueprintCallable)
	void Sell();
	UFUNCTION(BlueprintPure)
	const int32 GetSellingPrice() const;

	//pure virtual functions
	UFUNCTION(BlueprintCallable, Category = "Highlight")
	virtual void EnableHighlight() {};
	UFUNCTION(BlueprintCallable, Category = "Highlight")
	virtual void DisableHighlight() {};

	//Pure virtuals
	virtual const UStaticMeshComponent* GetMesh() const { return nullptr; };

	void SetSellingPrice(const int32 Price);

protected:

	void OnDeath() override;

	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	float EasyDamageMod = 1.2f;
	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	float HardDamageMod = 0.9f;
	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	float EasySellingPriceMod = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	float NormalSellingPriceMod = 0.8f;
	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	float HardSellingPriceMod = 0.5f;

	//Pure virtuals
	virtual void OnSpawnOverlap(TArray<AActor*> OverlapActors) {};

public:

	FOnPlaceableDeath OnPlaceableDeath;

private:

	int32 SellingPrice = 1;

};
