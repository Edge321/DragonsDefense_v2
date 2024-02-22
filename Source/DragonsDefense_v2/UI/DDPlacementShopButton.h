// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/DDInfiniteShopButton.h"
#include "DDPlacementShopButton.generated.h"

class ADDPlaceable;

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API UDDPlacementShopButton : public UDDInfiniteShopButton
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable)
	void SetPreviewAndPrice() const;

	UPROPERTY(EditAnywhere, Category = "Placeable")
	TSubclassOf<ADDPlaceable> PlaceableClass;

public:

	//Switches placement mode from false to true and vice versa
	UFUNCTION(BlueprintCallable)
	void PlacementMode(bool IsPlacing) const;

	
};
