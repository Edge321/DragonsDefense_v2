// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDPlaceableInfoWidget.generated.h"

class ADDPlaceable;
class ADDSentientPlaceable;
class ADDTrapPlaceable;
class UCanvasPanel;
class UCanvasPanelSlot;

enum class EPlaceableAI : uint8;

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API UDDPlaceableInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "Placeable")
	void SetCurrentPlaceable(ADDPlaceable* CurrentPlaceable);
	UFUNCTION(BlueprintCallable, Category = "PlaceableAI")
	void ChangeBehavior(const EPlaceableAI AI);
	void GetStats();

	UPROPERTY(BlueprintReadOnly, Category = "Placeable")
	ADDPlaceable* MostRecentPlaceable;
	UPROPERTY(BlueprintReadOnly, Category = "Sentient")
	ADDSentientPlaceable* Sentient;
	UPROPERTY(BlueprintReadOnly, Category = "Placeable")
	bool bIsSentient;

private:
	
	ADDTrapPlaceable* Trap;

};
