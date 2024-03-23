// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "DDCollapsibleCanvas.generated.h"

UENUM(BlueprintType)
enum class EPosition : uint8 {
	Left,
	Right,
	Top,
	Bottom
};

class UButton;
class UCanvasPanelSlot;

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API UDDCollapsibleCanvas : public UCanvasPanel
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "CollapseButton")
	void SetButtonPosition();
	UFUNCTION(BlueprintCallable, Category = "Canvas")
	void SetCanvasPosition();
	UFUNCTION(BlueprintCallable, Category = "Canvas")
	void SetOwnCanvasPosition(const FVector2D AnchorsMin, const FVector2D AnchorsMax, const FVector2D CanvasAlignment);
	UFUNCTION(BlueprintCallable, Category = "Canvas")
	void CollapseButtonClickEventFunction();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CollapseButton")
	UButton* CollapseButton;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Position")
	EPosition Position = EPosition::Top;

private:

	UFUNCTION(BlueprintCallable, Category = "CollapseButton")
	void ValidateButton();

	UCanvasPanelSlot* ButtonCollapseSlot;
	UCanvasPanelSlot* CanvasCollapseSlot;

	FVector2D OriginalAlignment;
	bool bIsCollapsed = false;

};
