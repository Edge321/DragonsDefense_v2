// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ADDGameModeBase;

struct FDDPlaceablePurchaseInfo 
{

public:

	bool IsBuyable() const;
	void UpdateSouls() const;
	void SetPrice(const int32 Price);
	void SetGameMode(ADDGameModeBase* Mode);
		
private:

	int32 CurrentPrice = 1;
	ADDGameModeBase* GameMode = nullptr;

};
