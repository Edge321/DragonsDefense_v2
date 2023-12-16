// Fill out your copyright notice in the Description page of Project Settings.


#include "DDMainMenuWidget.h"
#include "Components/CanvasPanel.h"
#include "Kismet/KismetSystemLibrary.h"
//My classes
#include "../Game/DDGameModeBase.h"

void UDDMainMenuWidget::Start()
{
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->GameStart();
	}
	SetVisibility(ESlateVisibility::Hidden);
}

void UDDMainMenuWidget::SwitchMenus(UCanvasPanel* MenuDest, UCanvasPanel* MenuSrc)
{
	if (MenuDest && MenuSrc) {
		if (MenuSrc->GetVisibility() == ESlateVisibility::Hidden) {
			UE_LOG(LogTemp, Warning, TEXT("Warning: Menu source is already hidden!"))
		}
		if (MenuSrc->GetVisibility() == ESlateVisibility::Visible) {
			UE_LOG(LogTemp, Warning, TEXT("Warning: Menu destination is already hidden!"))
		}

		MenuSrc->SetVisibility(ESlateVisibility::Hidden);
		MenuDest->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Error: %s is a null pointer. Check if it was assigned correctly"), 
			MenuDest ? "Menu Destination" : "Menu Source")
	}
	
}

void UDDMainMenuWidget::Quit() const
{
	UWorld* World = GetWorld();
	if (World) {
		UKismetSystemLibrary::QuitGame(World, nullptr, EQuitPreference::Quit, false);
	}
}
