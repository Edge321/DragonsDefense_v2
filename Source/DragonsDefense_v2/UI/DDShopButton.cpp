// Fill out your copyright notice in the Description page of Project Settings.


#include "DDShopButton.h"

void UDDShopButton::ValidatePriceText()
{
	if (PriceText == nullptr) {
		UE_LOG(LogTemp, Fatal, TEXT("Error: %s does not have a price text reference"), *GetName())
	}
}
//TODO - consider using OnHovered and all those binding in the code instead of blueprints
void UDDShopButton::InitializeButton()
{	
	UpdateText();
	SetBackgroundColor(UnBuyableColor);
	OnClicked.AddDynamic(this, &UDDShopButton::OnClickEventFunction);
	OnHovered.AddDynamic(this, &UDDShopButton::OnHoveredEventFunction);
	OnUnhovered.AddDynamic(this, &UDDShopButton::OnUnhoveredEventFunction);
}

const FString UDDShopButton::GetDescription() const
{
	return ButtonDescription;
}

void UDDShopButton::OnUnhoveredEventFunction()
{
	OnCustomUnhovered.Broadcast();
}

const bool UDDShopButton::GetBuyableStatus() const
{
	return bIsBuyable;
}

void UDDShopButton::GameOverEventFunction()
{
	bIsBuyable = false;
	SetIsEnabled(true);
	SetBackgroundColor(UnBuyableColor);
}
