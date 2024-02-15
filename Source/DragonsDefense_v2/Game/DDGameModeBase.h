// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DDGameModeBase.generated.h"

class UUserWidget;
class UDDScoreWidget;
class UDDMainMenuWidget;
class UDDSoulShopWidget;
class UDDHealthBarWidget;
class ADDProjectileManager;
class ADDEnemySpawner;
class ADDPlayer;
enum class EDifficulty : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateSouls);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveOver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveStart);
//TODO - consider the above... what classes have their feeling affected?

UCLASS()
class DRAGONSDEFENSE_V2_API ADDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TArray<TSubclassOf<class UUserWidget>> WidgetClassArray;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> ScoreWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> SoulShopWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> HealthBarWidgetClass;


	void UpdateScoreText();
	void UpdateSoulsText();

public:

	void AddScore(int32 Score);
	void UpdateSouls(int32 Souls);

	ADDProjectileManager& GetProjectileManager();
	ADDEnemySpawner& GetEnemySpawner();
	ADDPlayer& GetPlayer();
	
	UFUNCTION(BlueprintPure, Category = "Getters")
	const EDifficulty GetDifficulty() const;
	UFUNCTION(BlueprintCallable, Category = "Setters")
	void SetDifficulty(const EDifficulty NewDifficulty);
	UFUNCTION(BlueprintPure, Category = "Getters")
	const int32 GetSouls() const;
	UFUNCTION(BlueprintPure, Category = "Getters")
	UDDMainMenuWidget* GetMainMenuWidget() const;
	UFUNCTION(BlueprintPure, Category = "Getters")
	const bool GetPauseState();
	UFUNCTION(BlueprintCallable, Category = "Setters")
	void SetPauseState(const bool PauseState);
	UFUNCTION(BlueprintCallable, Category = "GameState")
	void GameOver();
	UFUNCTION(BlueprintCallable, Category = "GameState")
	void GameStart() const;
	UFUNCTION(BlueprintCallable, Category = "WaveState")
	void WaveOver() const;
	UFUNCTION(BlueprintCallable, Category = "WaveState")
	void WaveStart() const;

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnGameOver OnGameOver;
	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnGameStart OnGameStart;
	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnUpdateSouls OnUpdateSouls;
	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnWaveStart OnWaveStart;
	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnWaveOver OnWaveOver;

private:
	//Adds any subclass of UUserWidget to the viewport. Returns whatever class
	//that inherits from UUserWidget.
	template <class T> T* AddWidgetToViewport(TSubclassOf<UUserWidget> WidgetClass);
	//Searches for some kind of UObject in the world. Also checks if there is only
	//one instance of the object.
	template <class T> T* FindUObject();

	//Blueprint functions below are so C++ can't call it but blueprints can
	//since C++ can modify the pointers (even though you can change by reference
	//to pointer but shut up okay?)

	UFUNCTION(BlueprintCallable, Category = "Getters")
	ADDPlayer* BlueprintGetPlayer();
	UFUNCTION(BlueprintCallable, Category = "Getters")
	ADDProjectileManager* BlueprintGetProjectileManager();

	ADDProjectileManager* ProjectileManager;
	ADDEnemySpawner* EnemySpawner;
	ADDPlayer* Player;

	UDDScoreWidget* ScoreWidget;
	UDDMainMenuWidget* MainMenuWidget;
	UDDSoulShopWidget* SoulShopWidget;
	UDDHealthBarWidget* HealthBarWidget;

	EDifficulty Difficulty;

	int32 TotalEnemiesKilled = 0;
	int32 TotalSouls = 0;

	bool bIsPaused = false;
};