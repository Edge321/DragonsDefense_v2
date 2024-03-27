// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCollapsibleCanvas.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"

void UDDCollapsibleCanvas::SetButtonPosition()
{
	FVector2D Size, ButtonAlignment;
	FAnchors Anchors;
	
	ButtonCollapseSlot = Cast<UCanvasPanelSlot>(CollapseButton->Slot);	

	switch (Position) {
		case EPosition::Top:
			Anchors.Maximum = FVector2D(0.5, 1);
			Anchors.Minimum = FVector2D(0.5, 1);
			ButtonAlignment = FVector2D(0.5, 0);
			break;
		case EPosition::Bottom:
			Anchors.Maximum = FVector2D(0.5, 0);
			Anchors.Minimum = FVector2D(0.5, 0);
			ButtonAlignment = FVector2D(0.5, 1);
			break;
		case EPosition::Left:
			Anchors.Maximum = FVector2D(1, 0.5);
			Anchors.Minimum = FVector2D(1, 0.5);
			ButtonAlignment = FVector2D(0, 0.5);
			break;
		case EPosition::Right:
			Anchors.Maximum = FVector2D(0, 0.5);
			Anchors.Minimum = FVector2D(0, 0.5);
			ButtonAlignment = FVector2D(1, 0.5);
			break;
		default:
			UE_LOG(LogTemp, Fatal, TEXT("How did we get here?"))
	}

	if (ButtonCollapseSlot) {
		ButtonCollapseSlot->SetPosition(FVector2D(0, 0));
		ButtonCollapseSlot->SetAlignment(ButtonAlignment);
		ButtonCollapseSlot->SetAnchors(Anchors);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("CanvasSlot failed to cast"))
	}
}

void UDDCollapsibleCanvas::SetCanvasPosition()
{
	FVector2D CanvasAlignment;
	FAnchors Anchors;
	CanvasCollapseSlot = Cast<UCanvasPanelSlot>(Slot);

	switch (Position) {
	case EPosition::Top:
		Anchors.Maximum = FVector2D(0.5, 0);
		Anchors.Minimum = FVector2D(0.5, 0);
		CanvasAlignment = FVector2D(0.5, 0);
		OriginalAlignment = FVector2D(0.5, 0);
		break;
	case EPosition::Bottom:
		Anchors.Maximum = FVector2D(0.5, 1);
		Anchors.Minimum = FVector2D(0.5, 1);
		CanvasAlignment = FVector2D(0.5, 1);
		OriginalAlignment = FVector2D(0.5, 1);
		break;
	case EPosition::Left:
		Anchors.Maximum = FVector2D(0, 0.5);
		Anchors.Minimum = FVector2D(0, 0.5);
		CanvasAlignment = FVector2D(0, 0.5);
		OriginalAlignment = FVector2D(0, 0.5);
		break;
	case EPosition::Right:
		Anchors.Maximum = FVector2D(1, 0.5);
		Anchors.Minimum = FVector2D(1, 0.5);
		CanvasAlignment = FVector2D(1, 0.5);
		OriginalAlignment = FVector2D(1, 0.5);
		break;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("How did we get here?"))
	}

	if (CanvasCollapseSlot) {
		CanvasCollapseSlot->SetPosition(FVector2D(0, 0));
		CanvasCollapseSlot->SetAlignment(CanvasAlignment);
		CanvasCollapseSlot->SetAnchors(Anchors);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("CanvasSlot failed to cast"))
	}
}

void UDDCollapsibleCanvas::SetOwnCanvasPosition(const FVector2D AnchorsMin, const FVector2D AnchorsMax, const FVector2D CanvasAlignment)
{
	FAnchors Anchors;
	Anchors.Minimum = AnchorsMin;
	Anchors.Maximum = AnchorsMax;

	CanvasCollapseSlot = Cast<UCanvasPanelSlot>(Slot);

	if (CanvasCollapseSlot) {
		CanvasCollapseSlot->SetPosition(FVector2D(0, 0));
		CanvasCollapseSlot->SetAlignment(CanvasAlignment);
		CanvasCollapseSlot->SetAnchors(Anchors);
		OriginalAlignment = CanvasAlignment;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("CanvasSlot is nullptr"))
	}
}

void UDDCollapsibleCanvas::CollapseButtonClickEventFunction()
{
	FVector2D CanvasAlignment;

	if (bIsCollapsed) {
		bIsCollapsed = false;
		CanvasCollapseSlot->SetAlignment(OriginalAlignment);
	}
	else {
		bIsCollapsed = true;
		switch (Position) {
			case EPosition::Top:
				CanvasAlignment = FVector2D(0.5, 1);
				break;
			case EPosition::Bottom:
				CanvasAlignment = FVector2D(0.5, 0);
				break;
			case EPosition::Left:
				CanvasAlignment = FVector2D(1, 0.5);
				break;
			case EPosition::Right:
				CanvasAlignment = FVector2D(0, 0.5);
				break;
			default:
				UE_LOG(LogTemp, Fatal, TEXT("How did we get here?"));
		}
		CanvasCollapseSlot->SetAlignment(CanvasAlignment);
	}
	UE_LOG(LogTemp, Warning, TEXT("Canvas is being collapsed: %s"), bIsCollapsed ? TEXT("Yes") : TEXT("No"));
}

const bool UDDCollapsibleCanvas::IsCollapsed() const
{
	return bIsCollapsed;
}

void UDDCollapsibleCanvas::ValidateButton()
{
	check(CollapseButton);
}
