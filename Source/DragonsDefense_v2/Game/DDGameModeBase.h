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
class UDDSoulShopWidget;
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
	TArray<TSubclassOf<class UUserWidget>> WidgetClassArray;
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UUserWidget> ScoreWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UUserWidget> SoulShopWidgetClass;

	//So we have an array of widget classes. Wtf do I do for array of
	//various sorts of widgets like below?
	UPROPERTY()
	UDDScoreWidget* ScoreWidget;
	UPROPERTY()
	UDDMainMenuWidget* MainMenuWidget;
	UPROPERTY()
	UDDSoulShopWidget* SoulShopWidget;

	void UpdateScoreText();

public:

	void AddScore(int32 Score);

	ADDProjectileManager& GetProjectileManager();
	ADDPlayer& GetPlayer();
	
	UFUNCTION(BlueprintCallable, Category = "Getters")
	const int32 GetSouls();
	UFUNCTION(BlueprintCallable, Category = "Getters")
	UDDMainMenuWidget* GetMainMenuWidget();
	UFUNCTION(BlueprintCallable, Category = "GameState")
	void GameOver();
	UFUNCTION(BlueprintCallable, Category = "GameState")
	void GameStart() const;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnGameOver OnGameOver;
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnGameStart OnGameStart;

private:
	template <class T> T* AddWidgetToViewport();
	/**
	 * @brief Caches any object in GameMode. Also checks if there is only
	 * one instance of the object.
	 * 
	 */
	template <class T> T* FindUObject();

	ADDProjectileManager* ProjectileManager;
	ADDPlayer* Player;

	int32 Souls = 0;
};