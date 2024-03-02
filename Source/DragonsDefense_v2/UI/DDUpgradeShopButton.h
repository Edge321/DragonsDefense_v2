// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/DDShopButton.h"
#include "DDUpgradeShopButton.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API UDDUpgradeShopButton : public UDDShopButton
{
	GENERATED_BODY()

protected:
	//An array of increasing prices for when the player purchases whatever the button sells
	UPROPERTY(EditAnywhere, Category = "Prices")
	TArray<int32> Prices;
	UPROPERTY(EditAnywhere, Category = "UpgradeAmount")
	float UpgradeAmount = 1;

	void IsBuyable() override;
	UFUNCTION(BlueprintCallable)
	void Upgrade(EPlayerStats Stat) const;
	UFUNCTION(BlueprintCallable)
	void IncreasePrice();
	void UpdateSouls() override;
	void GameOverEventFunction() override;

private:
	void UpdateText() override;
	//Fetches the current price but is converted to FText
	bool IsMaxxedOut();
	void ResetPrice();
	FText FetchFTextPrice() override;

	int32 PriceIndex = 0;
};
