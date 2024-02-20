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

	UFUNCTION(BlueprintCallable)
	void SetPreviewMesh();

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMesh* ButtonMesh;

public:

	//Switches placement mode from false to true and vice versa
	UFUNCTION(BlueprintCallable)
	void PlacementMode(bool IsPlacing);

	
};
