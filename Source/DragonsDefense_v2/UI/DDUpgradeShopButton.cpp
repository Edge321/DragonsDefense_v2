// Fill out your copyright notice in the Description page of Project Settings.

#include "DDUpgradeShopButton.h"
//My classes
#include "../Characters/DDPlayerStats.h"
#include "../Characters/DDPlayer.h"

void UDDUpgradeShopButton::Upgrade(EPlayerStats Stat) const
{
	ADDPlayer* Player = nullptr;
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		//Doing some mumbo jumbo here as I want Player to be outside the if-statement scope
		Player = &(GameMode->GetPlayer());
	}

	check(Player)

	//TODO - Add damage upgrade!
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
		case EPlayerStats::Armor:
			Player->UpdateArmor(UpgradeAmount);
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Error: No upgrade set to %s. Bad things will occur"), *GetName())
			break;
	}
}
