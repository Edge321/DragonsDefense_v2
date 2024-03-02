// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "../Characters/DDPlaceableAI.h"
#include "DDAIButton.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API UDDAIButton : public UButton
{
	GENERATED_BODY()


protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EPlaceableAI PlaceableAI;

};
