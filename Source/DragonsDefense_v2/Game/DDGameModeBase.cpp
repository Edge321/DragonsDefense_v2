// Fill out your copyright notice in the Description page of Project Settings.


#include "DDGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
//My classes
#include "../Characters/DDPlayer.h"
#include "../Game/DDProjectileManager.h"
#include "../Game/DDScoreWidget.h"
#include "../UI/DDMainMenuWidget.h"
#include "../UI/DDSoulShopWidget.h"

void ADDGameModeBase::BeginPlay()
{
	//WidgetToViewport();
	//TODO - Bunch these widgets in an array of widgets, dont have to copy code that way
	if (ScoreWidgetClass) {
		ScoreWidget = Cast<UDDScoreWidget>(CreateWidget(GetWorld(), ScoreWidgetClass));

		if (ScoreWidget) {
			ScoreWidget->AddToViewport();
			UpdateScoreText(); //TODO - If we want to use a generic function, we need to find a way to get this bozo out of here
		}
	}

	if (MainMenuWidgetClass) {
		MainMenuWidget = Cast<UDDMainMenuWidget>(CreateWidget(GetWorld(), MainMenuWidgetClass));

		if (MainMenuWidget) {
			MainMenuWidget->AddToViewport();
		}
	}

	if (SoulShopWidgetClass) {
		SoulShopWidget = Cast<UDDSoulShopWidget>(CreateWidget(GetWorld(), SoulShopWidgetClass));

		if (SoulShopWidget) {
			SoulShopWidget->AddToViewport();
		}
	}

	ProjectileManager = FindUObject<ADDProjectileManager>();
	Player = FindUObject<ADDPlayer>();
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

ADDPlayer& ADDGameModeBase::GetPlayer()
{
	check(Player)
	return *Player;
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

void ADDGameModeBase::GameStart() const
{
	OnGameStart.Broadcast();
}

//void ADDGameModeBase::WidgetToViewport()
//{
//	for (TSubclassOf<UUserWidget> Widget : WidgetClassArray) {
//		UUserWidget* SomeWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), Widget));
//		if (SomeWidget) {
//			UpdateScoreText();
//		}
//	}
//}

template<class T>
T* ADDGameModeBase::AddWidgetToViewport()
{
	//T* SomeWidget = Cast<T>(CreateWidget(GetWorld(), Widget));
}

template<class T>
T* ADDGameModeBase::FindUObject()
{
	TArray<AActor*> ActorArray;
	if (UWorld* World = GetWorld()) {
		UGameplayStatics::GetAllActorsOfClass(World, T::StaticClass(), ActorArray);

		check(ActorArray.Num() == 1)
		check(!ActorArray.IsEmpty());

		for (AActor* Actor : ActorArray) {
			T* SomeActor = Cast<T>(Actor);
			if (SomeActor) {
				return SomeActor;
			}
		}
	}
	return nullptr;
}