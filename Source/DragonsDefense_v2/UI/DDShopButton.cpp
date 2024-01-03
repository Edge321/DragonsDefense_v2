// Fill out your copyright notice in the Description page of Project Settings.


#include "DDShopButton.h"
//My classes
#include "../Game/DDGameModeBase.h"

void UDDShopButton::UpdateText()
{
	PriceText->SetText(FetchFTextPrice());
}

void UDDShopButton::ValidatePriceText()
{
	check(PriceText)
}

void UDDShopButton::InitializeButton()
{
	UpdateText();
	SetBackgroundColor(UnBuyableColor);
}

bool UDDShopButton::IsMaxxedOut()
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

void UDDShopButton::IsBuyable()
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

void UDDShopButton::IncreasePrice()
{
	PriceIndex++;
	if (!IsMaxxedOut()) {
		UpdateText();
	}
}

void UDDShopButton::UpdateSouls()
{
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->UpdateSouls(-Prices[PriceIndex]);
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("How did we get here?"))
	}
}

const bool UDDShopButton::GetBuyableStatus()
{
	return bIsBuyable;
}

void UDDShopButton::ResetPrice()
{
	PriceIndex = 0;
	PriceText->SetText(FetchFTextPrice());
}
//TODO - Gonna have to think up of how the hell to call various buttons with
//different texts.
void UDDShopButton::GameOverEventFunction()
{
	//TODO - Bind to the game over delegate from GameModeBase
	//To initialize in the canvas, have the canvas gather a list of all the buttons that exist within the canvas
	ResetPrice();
	UpdateText();
	bIsBuyable = false;
	SetIsEnabled(true);
	SetBackgroundColor(UnBuyableColor);
}

FText UDDShopButton::FetchFTextPrice()
{
	FString PriceString = FString::FromInt(Prices[PriceIndex]);
	return FText::FromString(PriceString);
}
