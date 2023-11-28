// Fill out your copyright notice in the Description page of Project Settings.


#include "DDGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
//My classes
#include "../Game/DDScoreWidget.h"
#include "../Game/DDProjectileManager.h"

void ADDGameModeBase::BeginPlay()
{
	if (ScoreWidgetClass) {
		ScoreWidget = Cast<UDDScoreWidget>(CreateWidget(GetWorld(), ScoreWidgetClass));

		if (ScoreWidget) {
			ScoreWidget->AddToViewport();
			UpdateScoreText();
		}
	}
	FindProjectileManager();
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

ADDProjectileManager& ADDGameModeBase::GetProjectileManager()
{
	check(ProjectileManager)
	return *ProjectileManager;
}

void ADDGameModeBase::GameOver()
{
	TotalEnemiesKilled = 0;
	UpdateScoreText();
	OnGameOver.Broadcast();
}

void ADDGameModeBase::GameStart()
{
	OnGameStart.Broadcast();
}

void ADDGameModeBase::FindProjectileManager()
{
	TArray<AActor*> ProjectileManagersToFind;
	if (UWorld* World = GetWorld()) {
		UGameplayStatics::GetAllActorsOfClass(World, ADDProjectileManager::StaticClass(), ProjectileManagersToFind);

		check(ProjectileManagersToFind.Num() == 1)
		check(!ProjectileManagersToFind.IsEmpty());

		for (AActor* ProjManager : ProjectileManagersToFind) {
			ADDProjectileManager* SomeProjManager = Cast<ADDProjectileManager>(ProjManager);
			if (SomeProjManager) {
				ProjectileManager = SomeProjManager;
				break;
			}
		}
	}
}