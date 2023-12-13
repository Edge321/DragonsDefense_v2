// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DDGameModeBase.generated.h"

// TODO - Add an enum class for difficulties

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStart);

class UDDScoreWidget;
class UDDMainMenuWidget;
class ADDProjectileManager;
class ADDPlayer;

UCLASS()
class DRAGONSDEFENSE_V2_API ADDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	
	virtual void BeginPlay() override;

	int32 TotalEnemiesKilled = 0;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UUserWidget> ScoreWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;

	UPROPERTY()
	UDDScoreWidget* ScoreWidget;
	UPROPERTY()
	UDDMainMenuWidget* MainMenuWidget;

	void UpdateScoreText();

public:

	void AddScore(int32 Score);

	//TODO - Make a get player function
	ADDProjectileManager& GetProjectileManager();
	
	UFUNCTION(BlueprintCallable, Category = "Getters")
	const int32 GetSouls();
	UFUNCTION(BlueprintCallable, Category = "Getters")
	UDDMainMenuWidget* GetMainMenuWidget();
	UFUNCTION(BlueprintCallable, Category = "GameState")
	void GameOver();
	UFUNCTION(BlueprintCallable, Category = "GameState")
	void GameStart();

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnGameOver OnGameOver;
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnGameStart OnGameStart;

private:
	/**
	 * @brief Caches the projectile manager
	 * 
	 */
	void FindProjectileManager();

	template <class T> void FindUObject(T* ActualActor);

	ADDProjectileManager* ProjectileManager;
	ADDPlayer* Player;

	int32 Souls = 0;
};