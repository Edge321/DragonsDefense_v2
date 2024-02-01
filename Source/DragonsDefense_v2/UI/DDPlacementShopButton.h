// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/DDInfiniteShopButton.h"
#include "DDPlacementShopButton.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API UDDPlacementShopButton : public UDDInfiniteShopButton
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Placeable")
	TSubclassOf<AActor> Placeable; //TODO - Remove AActor with Placeable
	
	UFUNCTION(BlueprintCallable)
	void PlaceObject(); //NOTE - Might have to do this in blueprints
};
