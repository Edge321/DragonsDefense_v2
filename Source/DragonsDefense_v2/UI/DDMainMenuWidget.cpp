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

void UDDMainMenuWidget::SetDifficulty(EDifficulty Difficulty) const
{
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->SetDifficulty(Difficulty);
	}
}

void UDDMainMenuWidget::Quit() const
{
	UWorld* World = GetWorld();
	if (World) {
		UKismetSystemLibrary::QuitGame(World, nullptr, EQuitPreference::Quit, false);
	}
}
