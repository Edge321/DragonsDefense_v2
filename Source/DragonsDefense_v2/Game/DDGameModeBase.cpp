// Fill out your copyright notice in the Description page of Project Settings.


#include "DDGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
//My classes
#include "../Characters/DDPlayer.h"
#include "../Game/DDProjectileManager.h"
#include "../Game/DDScoreWidget.h"
#include "../Game/DDDifficulty.h"
#include "../Game/DDEnemySpawner.h"
#include "../UI/DDMainMenuWidget.h"
#include "../UI/DDSoulShopWidget.h"
#include "../UI/DDHealthBarWidget.h"

void ADDGameModeBase::BeginPlay()
{
	ScoreWidget = AddWidgetToViewport<UDDScoreWidget>(ScoreWidgetClass);
	MainMenuWidget = AddWidgetToViewport<UDDMainMenuWidget>(MainMenuWidgetClass);
	SoulShopWidget = AddWidgetToViewport<UDDSoulShopWidget>(SoulShopWidgetClass);
	HealthBarWidget = AddWidgetToViewport<UDDHealthBarWidget>(HealthBarWidgetClass);

	ProjectileManager = FindUObject<ADDProjectileManager>();
	Player = FindUObject<ADDPlayer>();
	EnemySpawner = FindUObject<ADDEnemySpawner>();
	Difficulty = EDifficulty::Normal;
}

void ADDGameModeBase::UpdateScoreText()
{
	ScoreWidget->SetScoreText(TotalEnemiesKilled);
}

void ADDGameModeBase::UpdateSoulsText()
{
	SoulShopWidget->UpdateSoulsText(TotalSouls);
}

void ADDGameModeBase::AddScore(int32 Score)
{
	TotalEnemiesKilled += Score;
	UpdateScoreText();
}

void ADDGameModeBase::UpdateSouls(int32 Souls)
{
	TotalSouls += Souls;
	UpdateSoulsText();
	OnUpdateSouls.Broadcast();
}

ADDProjectileManager& ADDGameModeBase::GetProjectileManager()
{
	check(ProjectileManager)
	return *ProjectileManager;
}

ADDEnemySpawner& ADDGameModeBase::GetEnemySpawner()
{
	check(EnemySpawner)
	return *EnemySpawner;
}

ADDPlayer& ADDGameModeBase::GetPlayer()
{
	check(Player)
	return *Player;
}

const EDifficulty ADDGameModeBase::GetDifficulty() const
{
	return Difficulty;
}

void ADDGameModeBase::SetDifficulty(const EDifficulty NewDifficulty)
{
	Difficulty = NewDifficulty;
}

const int32 ADDGameModeBase::GetSouls() const
{
	return TotalSouls;
}

UDDMainMenuWidget* ADDGameModeBase::GetMainMenuWidget() const
{
	check(MainMenuWidget)
	return MainMenuWidget;
}

const bool ADDGameModeBase::GetPauseState()
{
	return bIsPaused;
}

void ADDGameModeBase::SetPauseState(const bool PauseState)
{
	bIsPaused = PauseState;
}

void ADDGameModeBase::GameOver()
{
	TotalEnemiesKilled = 0;
	TotalSouls = 0;
	UpdateScoreText();
	UpdateSoulsText();
	OnGameOver.Broadcast();
}

void ADDGameModeBase::GameStart() const
{
	OnGameStart.Broadcast();
}

void ADDGameModeBase::WaveOver() const
{
	OnWaveOver.Broadcast();
}

void ADDGameModeBase::WaveStart() const
{
	OnWaveStart.Broadcast();
}

ADDPlayer* ADDGameModeBase::BlueprintGetPlayer()
{
	return Player;
}

ADDProjectileManager* ADDGameModeBase::BlueprintGetProjectileManager()
{
	return ProjectileManager;
}

template<class T>
T* ADDGameModeBase::AddWidgetToViewport(TSubclassOf<UUserWidget> WidgetClass)
{
	if (WidgetClass) {
		T* SomeWidget = Cast<T>(CreateWidget(GetWorld(), WidgetClass));
		if (SomeWidget) {
			SomeWidget->AddToViewport();
			return SomeWidget;
		}
		else {
			UE_LOG(LogTemp, Fatal, TEXT("Error: %s is not a subclass of UUserWidget"), *(SomeWidget->GetName()))
		}
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("Error: invalid WidgetClass argument"))
	}

	return nullptr;
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