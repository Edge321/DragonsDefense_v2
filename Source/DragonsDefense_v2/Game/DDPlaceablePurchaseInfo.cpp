// Fill out your copyright notice in the Description page of Project Settings.


#include "DDPlaceablePurchaseInfo.h"
//My classes
#include "../Game/DDGameModeBase.h"

bool FDDPlaceablePurchaseInfo::IsBuyable() const
{
	int32 Souls;

	if (GameMode) {
		Souls = GameMode->GetSouls();
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("Something went wrong with GameMode!"))
		return false;
	}

	if (Souls >= CurrentPrice) {
		return true;
	}
	else {
		return false;
	}
}

void FDDPlaceablePurchaseInfo::UpdateSouls() const
{
	if (GameMode) {
		GameMode->UpdateSouls(-CurrentPrice);
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("How did we get here?"))
	}
}

void FDDPlaceablePurchaseInfo::SetPrice(const int32 Price)
{
	CurrentPrice = Price;
}

const int32 FDDPlaceablePurchaseInfo::GetPrice() const
{
	return CurrentPrice;
}

void FDDPlaceablePurchaseInfo::SetGameMode(ADDGameModeBase* Mode)
{
	GameMode = Mode;
}

