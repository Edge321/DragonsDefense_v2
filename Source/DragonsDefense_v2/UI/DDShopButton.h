// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../Characters/DDPlayerStats.h"
#include "../Characters/DDPlayer.h"
#include "../Game/DDGameModeBase.h"
#include "DDShopButton.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class DRAGONSDEFENSE_V2_API UDDShopButton : public UButton
{
	GENERATED_BODY()

protected:

	//Text for displaying the current price of the shop item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PriceText")
	UTextBlock* PriceText;
	UPROPERTY(EditAnywhere, Category = "ButtonColor")
	FLinearColor BuyableColor = FLinearColor::Blue;
	UPROPERTY(EditAnywhere, Category = "ButtonColor")
	FLinearColor UnBuyableColor = FLinearColor::Red;

	bool bIsBuyable = false;

public:

	//Checks if the PriceText variable was set. Otherwise, this shit would be pointless!!!!
	UFUNCTION(BlueprintCallable)
	void ValidatePriceText();
	UFUNCTION(BlueprintCallable)
	void InitializeButton();
	UFUNCTION(BlueprintCallable)
	virtual void IsBuyable() {}; //pure virtual
	UFUNCTION(BlueprintCallable)
	virtual void UpdateSouls() {}; //pure virtual
	UFUNCTION(BlueprintPure)
	const bool GetBuyableStatus() const;
	UFUNCTION(BlueprintCallable)
	virtual void GameOverEventFunction();

protected:

	virtual void UpdateText() {}; //pure virtual
	virtual FText FetchFTextPrice() { return FText(); }; //pure virtual
};
