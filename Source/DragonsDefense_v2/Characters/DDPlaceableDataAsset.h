// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DDPlaceableDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class DRAGONSDEFENSE_V2_API UDDPlaceableDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	FString PlaceableName = "Placeable";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	FString PlaceableDescription = "Description";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float Damage = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float Health = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float AttackSpeed = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float AttackRadius = -1;

};
