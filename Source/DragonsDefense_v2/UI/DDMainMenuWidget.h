// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDMainMenuWidget.generated.h"

class UCanvasPanel;
enum class EDifficulty : uint8;

UCLASS()
class DRAGONSDEFENSE_V2_API UDDMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	UFUNCTION(BlueprintCallable)
	void Start();
	UFUNCTION(BlueprintCallable)
	void SetDifficulty(EDifficulty Difficulty) const;
	UFUNCTION(BlueprintCallable)
	void Quit() const;
};
