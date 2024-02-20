// Fill out your copyright notice in the Description page of Project Settings.


#include "DDPlacementShopButton.h"
//My classes
#include "../Game/DDGameModeBase.h"
#include "../Game/DDPlaceableManager.h"

void UDDPlacementShopButton::PlacementMode(bool IsPlacing)
{
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());

	if (GameMode) {
		GameMode->IsPlacing(IsPlacing);
	}
}

void UDDPlacementShopButton::SetPreviewMesh()
{
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());

	if (GameMode) {
		ADDPlaceableManager& Manager = GameMode->GetPlaceableManager();
		Manager.ChangePreviewMesh(ButtonMesh);
	}
}
