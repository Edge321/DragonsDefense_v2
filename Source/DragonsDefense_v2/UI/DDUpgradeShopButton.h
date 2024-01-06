// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/DDShopButton.h"
#include "DDUpgradeShopButton.generated.h"

enum class EPlayerStats : uint8;

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API UDDUpgradeShopButton : public UDDShopButton
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "UpgradeAmount");
	float UpgradeAmount = 1;

	UFUNCTION(BlueprintCallable)
	void Upgrade(EPlayerStats Stat) const;
};
