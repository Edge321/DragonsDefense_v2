// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DDWidgetBlueprintLibrary.generated.h"

class UCanvasPanel;

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API UDDWidgetBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Widget")
	static void SwitchMenus(UCanvasPanel* MenuDest, UCanvasPanel* MenuSrc);
	//UFUNCTION(BlueprintCallable, Category = "Widget")
	//static void 
};
