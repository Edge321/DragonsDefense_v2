// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/DDShopButton.h"
#include "DDInfiniteShopButton.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class DRAGONSDEFENSE_V2_API UDDInfiniteShopButton : public UDDShopButton
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable)
	void DisableButton();
	UFUNCTION(BlueprintCallable)
	void EnableButton();

	UPROPERTY(EditAnywhere, Category = "Price")
	int32 Price = 0;
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsAuthorized = true;

	void UpdateSouls() override;
	void UpdateText() override;
	void IsBuyable() override;
	FText FetchFTextPrice() override;


};
