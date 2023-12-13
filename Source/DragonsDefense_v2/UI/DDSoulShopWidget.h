// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDSoulShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API UDDSoulShopWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	UFUNCTION(BlueprintCallable)
	void IncreasePrice();
	UFUNCTION(BlueprintCallable)
	void IsBuyable();
	UFUNCTION(BlueprintCallable)
	void DisplayStats();


};
