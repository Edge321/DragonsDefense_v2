// Fill out your copyright notice in the Description page of Project Settings.


#include "DDShopButton.h"
//My classes
#include "../Game/DDGameModeBase.h"

void UDDShopButton::ValidatePriceText()
{
	check(PriceText)
}

void UDDShopButton::IsMaxxedOut()
{
	if (PriceIndex + 1 > Prices.Num()) {
		PriceText->SetText(FText::FromString("Another day, another victory for the OGs"));
		SetIsEnabled(false);
		//TODO - Change color of button to maxxed out color
	}
}

const bool UDDShopButton::IsBuyable()
{
	int32 Souls;

	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		Souls = GameMode->GetSouls();
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("Something went wrong with fetching GameMode!"))
		return false;
	}

	if (Souls > Prices[PriceIndex]) {
		return true;
	}
	else {
		return false;
	}

}

void UDDShopButton::IncreasePrice()
{
	PriceIndex++;
	IsMaxxedOut();
}

void UDDShopButton::ResetPrice()
{
	PriceIndex = 0;
}

void UDDShopButton::GameOverEventHandle()
{
	//TODO - Bind to the game over delegate from GameModeBase
}
