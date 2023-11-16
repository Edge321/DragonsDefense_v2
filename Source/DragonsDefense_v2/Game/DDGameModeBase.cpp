// Fill out your copyright notice in the Description page of Project Settings.


#include "DDGameModeBase.h"
#include "../Game/DDScoreWidget.h"
#include "Blueprint/UserWidget.h"

void ADDGameModeBase::BeginPlay()
{
	if (ScoreWidgetClass) {
		ScoreWidget = Cast<UDDScoreWidget>(CreateWidget(GetWorld(), ScoreWidgetClass));

		if (ScoreWidget) {
			ScoreWidget->AddToViewport();
			UpdateScoreText();
		}
	}
}

void ADDGameModeBase::UpdateScoreText()
{
	ScoreWidget->SetScoreText(TotalEnemiesKilled);
}

void ADDGameModeBase::AddScore(int32 Score)
{
	TotalEnemiesKilled += Score;
	UpdateScoreText();
}

void ADDGameModeBase::AddToActorPool(AActor* Actor)
{
	ActorPool.Add(Actor);
}

void ADDGameModeBase::RemoveActorFromPool(AActor* Actor)
{
	int32 removed = ActorPool.Remove(Actor);
}

void ADDGameModeBase::GameOver()
{
	RemoveAllActors();
	OnGameOver.Broadcast();
	//TODO - Reset whatever object needs to be reset
}

void ADDGameModeBase::RemoveAllActors()
{
	for (AActor* Actor : ActorPool) {
		Actor->Destroy();
	}
	ActorPool.Empty();
}
