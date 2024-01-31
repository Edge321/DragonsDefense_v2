// Fill out your copyright notice in the Description page of Project Settings.

#include "DDUpgradeShopButton.h"

void UDDUpgradeShopButton::UpdateText()
{
	PriceText->SetText(FetchFTextPrice());
}

bool UDDUpgradeShopButton::IsMaxxedOut()
{
	bool MaxxedOut = false;
	if (PriceIndex + 1 > Prices.Num()) {
		PriceText->SetText(FText::FromString("MAX"));
		SetIsEnabled(false);
		MaxxedOut = true;
		SetBackgroundColor(UnBuyableColor);
	}

	return MaxxedOut;
}

void UDDUpgradeShopButton::IsBuyable()
{
	int32 Souls;

	if (IsMaxxedOut()) {
		return;
	}

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

	if (Souls >= Prices[PriceIndex]) {
		bIsBuyable = true;
		SetBackgroundColor(BuyableColor);
	}
	else {
		bIsBuyable = false;
		SetBackgroundColor(UnBuyableColor);
	}

}

void UDDUpgradeShopButton::Upgrade(EPlayerStats Stat) const
{
	ADDPlayer* Player = nullptr;
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		//Doing some mumbo jumbo here as I want Player to be outside the if-statement scope
		Player = &(GameMode->GetPlayer());
	}

	check(Player)

	switch (Stat) {
		case EPlayerStats::MovementSpeed:
			Player->UpdateMovementSpeed(UpgradeAmount);
			break;
		case EPlayerStats::ShootSpeed:
			Player->UpdateShootSpeed(UpgradeAmount);
			break;
		case EPlayerStats::MaxHealth:
			Player->UpdateMaxHealth(UpgradeAmount);
			break;
		case EPlayerStats::Health:
			Player->UpdateHealth(UpgradeAmount);
			break;
		case EPlayerStats::Damage:
			Player->UpdateDamage(UpgradeAmount);
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Error: No upgrade set to %s. Bad things will occur"), *GetName())
			break;
	}
}

void UDDUpgradeShopButton::IncreasePrice()
{
	PriceIndex++;
	if (!IsMaxxedOut()) {
		UpdateText();
	}
}

void UDDUpgradeShopButton::UpdateSouls()
{
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		IncreasePrice();
		GameMode->UpdateSouls(-Prices[PriceIndex - 1]);
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("How did we get here?"))
	}
}

void UDDUpgradeShopButton::GameOverEventFunction()
{
	ResetPrice();
	UpdateText();
	bIsBuyable = false;
	SetIsEnabled(true);
	SetBackgroundColor(UnBuyableColor);
}

void UDDUpgradeShopButton::ResetPrice()
{
	PriceIndex = 0;
	PriceText->SetText(FetchFTextPrice());
}

FText UDDUpgradeShopButton::FetchFTextPrice()
{
	FString PriceString = FString::FromInt(Prices[PriceIndex]);
	return FText::FromString(PriceString);
}