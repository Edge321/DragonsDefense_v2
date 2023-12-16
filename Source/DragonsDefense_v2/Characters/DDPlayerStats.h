// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPlayerStats : uint8 {
	MovementSpeed,
	ShootSpeed,
	MaxHealth,
	Health,
	Armor
};
