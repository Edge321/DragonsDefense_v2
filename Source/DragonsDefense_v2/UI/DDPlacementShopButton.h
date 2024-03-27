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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placeable")
	TSubclassOf<ADDPlaceable> PlaceableClass;

	void SetPreviewAndPrice() const;
	void OnClickEventFunction() override;
	void OnHoveredEventFunction() override;
	//Set placement mode
	void PlacementMode(bool IsPlacing) const;

};
