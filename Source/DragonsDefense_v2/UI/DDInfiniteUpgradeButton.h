// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/DDInfiniteShopButton.h"
#include "DDInfiniteUpgradeButton.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API UDDInfiniteUpgradeButton : public UDDInfiniteShopButton
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "UpgradeAmount")
	float UpgradeAmount = 1;

	UFUNCTION(BlueprintCallable)
	void Upgrade(EPlayerStats Stat);
	void IsBuyable() override;
	void UpdateSouls() override;

};
