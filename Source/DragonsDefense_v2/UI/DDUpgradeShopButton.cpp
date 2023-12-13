// Fill out your copyright notice in the Description page of Project Settings.

#include "DDUpgradeShopButton.h"
//My classes
#include "../Characters/DDPlayerStats.h"

void UDDUpgradeShopButton::Upgrade(EPlayerStats Stat)
{
	//TODO - Get player's stat somehow. Maybe through blueprint
	switch (Stat) {
		case EPlayerStats::MovementSpeed:
			break;
		case EPlayerStats::ShootSpeed:
			break;
		case EPlayerStats::Health:
			break;
		case EPlayerStats::Armor:
			break;
		default:
			break;
	}
}
