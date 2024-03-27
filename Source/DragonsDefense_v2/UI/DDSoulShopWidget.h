// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDSoulShopWidget.generated.h"

class UDDShopButton;
class UDDUpgradeShopButton;
class UCanvasPanel;
enum class EPlayerStats : uint8;

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API UDDSoulShopWidget : public UUserWidget
{
	GENERATED_BODY()

protected:


	UFUNCTION(BlueprintCallable)
	TArray<UDDShopButton*> FindAllShopButtons(UCanvasPanel* Canvas);
	UFUNCTION(BlueprintCallable)
	void UpdateButtonsOfSameCategory(UDDUpgradeShopButton* Button);

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Event")
	void UpdateSoulsText(int32 Souls);

private:

	void AddToButtonMap(UDDUpgradeShopButton* Button);
	void RecursiveFindButtons(UWidget* Widget, TArray<UDDShopButton*>& ButtonArray);
	
	TMap<EPlayerStats, TArray<UDDUpgradeShopButton*>> UpgradeButtonsByCategory;

};
