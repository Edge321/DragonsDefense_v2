// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "DDShopButton.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class DRAGONSDEFENSE_V2_API UDDShopButton : public UButton
{
	GENERATED_BODY()

protected:
	
	void UpdateText();

	//Text for displaying the current price of the shop item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PriceText")
	UTextBlock* PriceText;

	//An array of increasing prices for when the player purchases whatever the button sells
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prices")
	TArray<int32> Prices;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonColor")
	FLinearColor BuyableColor = FLinearColor::Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonColor")
	FLinearColor UnBuyableColor = FLinearColor::Red;

public:

	//Checks if the PriceText variable was set. Otherwise, this shit would be pointless!!!!
	UFUNCTION(BlueprintCallable)
	void ValidatePriceText();
	UFUNCTION(BlueprintCallable)
	void InitializeButton();
	UFUNCTION(BlueprintCallable)
	bool IsMaxxedOut();
	UFUNCTION(BlueprintCallable)
	void IsBuyable();
	UFUNCTION(BlueprintCallable)
	void IncreasePrice();
	UFUNCTION(BlueprintCallable)
	void UpdateSouls();
	UFUNCTION(BlueprintCallable)
	const bool GetBuyableStatus();
	UFUNCTION(BlueprintCallable)
	void ResetPrice();
	UFUNCTION(BlueprintCallable)
	void GameOverEventFunction();

private:

	FText FetchFTextPrice();

	int PriceIndex = 0;
	bool bIsBuyable = false;
};
