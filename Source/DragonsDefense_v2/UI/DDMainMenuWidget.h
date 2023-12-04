// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDMainMenuWidget.generated.h"

class UCanvasPanel;

UCLASS()
class DRAGONSDEFENSE_V2_API UDDMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	UFUNCTION(BlueprintCallable)
	void Start();
	/**
	 * @brief Switches the menus the player is currently on
	 * 
	 * @param MenuDest Menu the player is going to
	 * @param MenuSrc Menu the player is coming from
	 */
	UFUNCTION(BlueprintCallable)
	void SwitchMenus(UCanvasPanel* MenuDest, UCanvasPanel* MenuSrc);
	UFUNCTION(BlueprintCallable)
	void Quit() const;
};
