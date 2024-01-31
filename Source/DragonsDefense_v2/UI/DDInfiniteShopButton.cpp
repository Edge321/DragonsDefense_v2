// Fill out your copyright notice in the Description page of Project Settings.


#include "DDInfiniteShopButton.h"

void UDDInfiniteShopButton::UpdateSouls()
{
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->UpdateSouls(Price);
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("How did we get here?"))
	}
}

void UDDInfiniteShopButton::UpdateText()
{
	PriceText->SetText(FetchFTextPrice());
}

FText UDDInfiniteShopButton::FetchFTextPrice()
{
	FString PriceString = FString::FromInt(Price);
	return FText::FromString(PriceString);
}
