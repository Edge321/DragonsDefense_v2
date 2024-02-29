// Fill out your copyright notice in the Description page of Project Settings.


#include "DDPlaceableInfoWidget.h"
//My classes
#include "../Characters/DDPlaceable.h"
#include "../Characters/DDSentientPlaceable.h"
#include "../Characters/DDTrapPlaceable.h"

void UDDPlaceableInfoWidget::SetCurrentPlaceable(ADDPlaceable* CurrentPlaceable)
{
	if (CurrentPlaceable) {
		if (CurrentPlaceable->IsA<ADDSentientPlaceable>()) {
			bIsSentient = true;
			Sentient = Cast<ADDSentientPlaceable>(CurrentPlaceable);
		}
		else {
			bIsSentient = false;
			Trap = Cast<ADDTrapPlaceable>(CurrentPlaceable);
		}
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("Placeable that was passed in is null!"))
	}
}

void UDDPlaceableInfoWidget::ChangeBehavior(const PlaceableAI AI)
{
	if (bIsSentient) {
		Sentient->SetAI(AI);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Attempted to modify TrapPlaceable"))
	}
}

void UDDPlaceableInfoWidget::GetStats()
{

}

