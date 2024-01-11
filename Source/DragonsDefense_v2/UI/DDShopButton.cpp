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
	if (PriceText == nullptr) {
		UE_LOG(LogTemp, Fatal, TEXT("Error: %s does not have a price text reference"), *GetName())
	}
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
// BUG - There existed a time where gathering a bunch of souls and buying resulted in being able
// to buy another upgrade where you didnt have enough souls. Not sure how.
// Update - It seems to exist only with the Damage button i think. Some bullshit
// Another update - not the damage button, but worse, math. More bullshit
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
		IncreasePrice();
		GameMode->UpdateSouls(-Prices[PriceIndex - 1]);
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("How did we get here?"))
	}
}

const bool UDDShopButton::GetBuyableStatus() const
{
	return bIsBuyable;
}

void UDDShopButton::ResetPrice()
{
	PriceIndex = 0;
	PriceText->SetText(FetchFTextPrice());
}

void UDDShopButton::GameOverEventFunction()
{
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
