// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UShapeComponent;

/**
 * 
 */
class DRAGONSDEFENSE_V2_API DDColliderLibrary
{

#define ECC_EnemyChannel ECC_GameTraceChannel1
#define ECC_PlaceableChannel ECC_GameTraceChannel2
#define ECC_PreviewChannel ECC_GameTraceChannel3
#define ECC_AttackRadiusChannel ECC_GameTraceChannel4

public:

	//Makes the collider ignore the passed in channel
	static void SetCollisionChannelToIgnore(UShapeComponent* Collider, const ECollisionChannel Channel);
};
