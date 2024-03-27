// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../Characters/DDPlayerStats.h"
#include "../Characters/DDPlayer.h"
#include "../Game/DDGameModeBase.h"
#include "DDShopButton.generated.h"

class ADDPlaceable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlaceableHovered, UDDShopButton*, Button, TSubclassOf<ADDPlaceable>, PlaceableClass);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeHovered, UDDShopButton*, Button);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomUnhovered);

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
	UPROPERTY(EditAnywhere, Category = "Description")
	FString ButtonDescription = "Insert Description";

	bool bIsBuyable = false;

public:

	//Checks if the PriceText variable was set. Otherwise, this shit would be pointless!!!!
	UFUNCTION(BlueprintCallable)
	void ValidatePriceText();
	UFUNCTION(BlueprintCallable)
	void InitializeButton();
	UFUNCTION(BlueprintPure)
	const FString GetDescription() const;
	UFUNCTION(BlueprintPure)
	const bool GetBuyableStatus() const;
	UFUNCTION(BlueprintCallable)
	virtual void GameOverEventFunction();
	UFUNCTION(BlueprintCallable)
	virtual void IsBuyable() {}; //pure virtual

	UPROPERTY(BlueprintAssignable)
	FOnPlaceableHovered OnPlaceableHovered;
	UPROPERTY(BlueprintAssignable)
	FOnUpgradeHovered OnUpgradeHovered;
	UPROPERTY(BlueprintAssignable)
	FOnCustomUnhovered OnCustomUnhovered;

protected:

	UFUNCTION()
	virtual void OnClickEventFunction() {}; //pure virtual
	UFUNCTION()
	virtual void OnHoveredEventFunction() {}; //pure virtual
	
	virtual void UpdateSouls() {}; //pure virtual
	virtual void UpdateText() {}; //pure virtual
	virtual FText FetchFTextPrice() { return FText(); }; //pure virtual

private:

	UFUNCTION()
	void OnUnhoveredEventFunction();

};
