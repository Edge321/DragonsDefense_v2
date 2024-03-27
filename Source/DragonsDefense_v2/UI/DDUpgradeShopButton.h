// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/DDShopButton.h"
#include "DDUpgradeShopButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgrade, UDDUpgradeShopButton*, Button);

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
	UPROPERTY(EditAnywhere, Category = "Upgrade|UpgradeAmount")
	float UpgradeAmount = 1;
	UPROPERTY(EditAnywhere, Category = "Upgrade")
	EPlayerStats StatUpgrade = EPlayerStats::Damage;

	void IsBuyable() override;
	void Upgrade(EPlayerStats Stat) const;
	void UpdateSouls() override;
	void GameOverEventFunction() override;
	void OnClickEventFunction() override;
	void OnHoveredEventFunction() override;

public:

	UFUNCTION(BlueprintPure)
	const EPlayerStats GetStatUpgrade() const;
	UFUNCTION(BlueprintCallable)
	void IncreasePrice();

	UPROPERTY(BlueprintAssignable)
	FOnUpgrade OnUpgrade;

private:
	void UpdateText() override;
	//Fetches the current price but is converted to FText
	bool IsMaxxedOut();
	void ResetPrice();
	FText FetchFTextPrice() override;

	int32 PriceIndex = 0;
};
