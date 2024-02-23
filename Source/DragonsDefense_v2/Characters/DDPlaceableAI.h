// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class PlaceableAI : uint8 {
	Deactivated,
	ClosestEnemy,
	FurthestEnemy,
	ClosestToCastle,
	FurthestFromCastle,
	RoundRobin,
	CurrentAttacker
};