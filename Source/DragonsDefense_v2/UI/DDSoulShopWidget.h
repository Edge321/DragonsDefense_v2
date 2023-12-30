// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDSoulShopWidget.generated.h"

class UDDShopButton;
class UCanvasPanel;

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API UDDSoulShopWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable)
	void DisplayStats();
	UFUNCTION(BlueprintCallable)
	TArray<UDDShopButton*> FindAllShopButtons(UCanvasPanel* Canvas);

private:

	void RecursiveFindButtons(UWidget* Widget, TArray<UDDShopButton*>& ButtonArray);

};
