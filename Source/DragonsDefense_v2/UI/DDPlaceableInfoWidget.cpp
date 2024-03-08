// Fill out your copyright notice in the Description page of Project Settings.


#include "DDPlaceableInfoWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
//My classes
#include "../Characters/DDPlaceable.h"
#include "../Characters/DDSentientPlaceable.h"
#include "../Characters/DDTrapPlaceable.h"

void UDDPlaceableInfoWidget::SetCurrentPlaceable(ADDPlaceable* CurrentPlaceable)
{
	if (CurrentPlaceable) {
		//If clicked on placeable is the same as currently stored one
		if (CurrentPlaceable == MostRecentPlaceable) {
			return;
		}

		MostRecentPlaceable = CurrentPlaceable;

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

void UDDPlaceableInfoWidget::ChangeBehavior(const EPlaceableAI AI)
{
	if (bIsSentient) {
		Sentient->SetAI(AI);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Attempted to modify TrapPlaceable"))
	}
}

FVector2D UDDPlaceableInfoWidget::CheckOutOfBounds(const FVector2D PanelSize, const FVector2D CursorLocation) const
{
	FVector2D DesiredPosition = CursorLocation;
	//Really make sure widget is adjusted correctly
	float Insurance = 50.0f;
	//Half since position of cursor in the middle of the panel
	FVector2D HalfPanel = PanelSize / 2;
	FVector2D ScreenSize;

	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(ScreenSize);
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("Something went wrong with GEngine!"))
	}

	//Adjust on X-axis
	if (CursorLocation.X + HalfPanel.X > ScreenSize.X) {
		DesiredPosition.X = ScreenSize.X - (HalfPanel.X + Insurance);
	}
	else if (CursorLocation.X - HalfPanel.X < 0) {
		DesiredPosition.X = HalfPanel.X + Insurance;
	}
	//Adjust on Y-axis
	if (CursorLocation.Y + HalfPanel.Y > ScreenSize.Y) {
		DesiredPosition.Y = ScreenSize.Y - (HalfPanel.Y + Insurance);
	}
	else if (CursorLocation.Y - HalfPanel.Y < 0) {
		DesiredPosition.Y = HalfPanel.Y + Insurance;
	}

	return DesiredPosition;
}

void UDDPlaceableInfoWidget::GetStats()
{

}

