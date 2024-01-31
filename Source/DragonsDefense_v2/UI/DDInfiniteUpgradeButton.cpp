// Fill out your copyright notice in the Description page of Project Settings.


#include "DDInfiniteUpgradeButton.h"

void UDDInfiniteUpgradeButton::Upgrade(EPlayerStats Stat)
{
	ADDPlayer* Player = nullptr;
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		//Doing some mumbo jumbo here as I want Player to be outside the if-statement scope
		Player = &(GameMode->GetPlayer());
	}

	//TODO - Add limitation on if health is full, prevent health upgrade being bought
	switch (Stat) {
	case EPlayerStats::Health:
		Player->UpdateHealth(UpgradeAmount);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Error: No upgrade set to %s. Bad things will occur"), *GetName())
			break;
	}
}

void UDDInfiniteUpgradeButton::IsBuyable()
{
	int32 Souls;

	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		Souls = GameMode->GetSouls();
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("Something went wrong with fetching GameMode!"))
			bIsBuyable = false;
		SetBackgroundColor(UnBuyableColor);
		return;
	}

	if (Souls >= Price) {
		bIsBuyable = true;
		SetBackgroundColor(BuyableColor);
	}
	else {
		bIsBuyable = false;
		SetBackgroundColor(UnBuyableColor);
	}
}

void UDDInfiniteUpgradeButton::UpdateSouls()
{
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->UpdateSouls(-Price);
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("How did we get here?"))
	}
}
