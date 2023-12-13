// Fill out your copyright notice in the Description page of Project Settings.


#include "DDGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
//My classes
#include "../Game/DDScoreWidget.h"
#include "../Characters/DDPlayer.h"
#include "../Game/DDProjectileManager.h"
#include "../UI/DDMainMenuWidget.h"

void ADDGameModeBase::BeginPlay()
{
	//TODO - Bunch these widgets in an array of widgets, dont have to copy code that way
	if (ScoreWidgetClass) {
		ScoreWidget = Cast<UDDScoreWidget>(CreateWidget(GetWorld(), ScoreWidgetClass));

		if (ScoreWidget) {
			ScoreWidget->AddToViewport();
			UpdateScoreText();
		}
	}

	if (MainMenuWidgetClass) {
		MainMenuWidget = Cast<UDDMainMenuWidget>(CreateWidget(GetWorld(), MainMenuWidgetClass));

		if (MainMenuWidget) {
			MainMenuWidget->AddToViewport();
		}
	}

	//FindProjectileManager();
	FindUObject<ADDProjectileManager>(ProjectileManager);
	FindUObject<ADDPlayer>(Player);
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

const int32 ADDGameModeBase::GetSouls()
{
	return Souls;
}

UDDMainMenuWidget* ADDGameModeBase::GetMainMenuWidget()
{
	check(MainMenuWidget)
	return MainMenuWidget;
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

//BUG - This generic function is NOT working!!!!
template<class T>
inline void ADDGameModeBase::FindUObject(T* ActualActor)
{
	TArray<AActor*> ActorArray;
	if (UWorld* World = GetWorld()) {
		UGameplayStatics::GetAllActorsOfClass(World, T::StaticClass(), ActorArray);

		check(ActorArray.Num() == 1)
		check(!ActorArray.IsEmpty());

		for (AActor* Actor : ActorArray) {
			T* SomeActor = Cast<T>(Actor);
			if (SomeActor) {
				ActualActor = SomeActor;
				break;
			}
		}
	}
}