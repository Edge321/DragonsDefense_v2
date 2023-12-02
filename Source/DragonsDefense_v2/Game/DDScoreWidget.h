// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDScoreWidget.generated.h"

UCLASS()
class DRAGONSDEFENSE_V2_API UDDScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetScoreText(int32 EnemiesKilled);
	UFUNCTION(BlueprintImplementableEvent)
	void ResetScoreText();
};
