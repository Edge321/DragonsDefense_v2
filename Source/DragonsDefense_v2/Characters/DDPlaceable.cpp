// Fill out your copyright notice in the Description page of Project Settings.


#include "DDPlaceable.h"
//My classes
#include "../Game/DDGameModeBase.h"
#include "../Game/DDDifficulty.h"
#include "../Lib/DDColliderLibrary.h"

// Sets default values
ADDPlaceable::ADDPlaceable()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Collider->SetCollisionObjectType(ECC_PlaceableChannel);
	DDColliderLibrary::SetCollisionChannelToIgnore(Collider, ECC_PlaceableChannel);
}

void ADDPlaceable::SetSellingPrice(const int32 Price)
{
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		EDifficulty Difficulty = GameMode->GetDifficulty();

		switch (Difficulty) {
		case EDifficulty::Easy:
			SellingPrice = FMath::Clamp(Price * EasySellingPriceMod, 1, Price);
			break;
		case EDifficulty::Normal:
			SellingPrice = FMath::Clamp(Price * NormalSellingPriceMod, 1, Price);
			break;
		case EDifficulty::Hard:
			SellingPrice = FMath::Clamp(Price * HardSellingPriceMod, 1, Price);
			break;
		}
	}
}

const int32 ADDPlaceable::GetSellingPrice() const
{
	return SellingPrice;
}

const FString ADDPlaceable::GetPlaceableName() const
{
	return PlaceableName;
}

const FString ADDPlaceable::GetDescription() const
{
	return PlaceableDescription;
}

void ADDPlaceable::OnConstruction(const FTransform& Transform)
{
	if (PlaceableData) {
		PlaceableData->PlaceableName = PlaceableName;
		PlaceableData->PlaceableDescription = PlaceableDescription;
		PlaceableData->Damage = -Damage;
		PlaceableData->Health = Health;
	}
}

void ADDPlaceable::Sell()
{
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->UpdateSouls(SellingPrice);
	}

	OnPlaceableDeath.ExecuteIfBound(this);
}

void ADDPlaceable::OnDeath()
{
	OnPlaceableDeath.ExecuteIfBound(this);
}
