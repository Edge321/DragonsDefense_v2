// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DDGameModeBase.generated.h"

// TODO - Add an enum class for difficulties

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStart);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateSouls);

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
	void UpdateSoulsText();

public:

	void AddScore(int32 Score);
	void UpdateSouls(int32 Souls);

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

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnGameOver OnGameOver;
	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnGameStart OnGameStart;
	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnUpdateSouls OnUpdateSouls;

private:
	template <class T> T* AddWidgetToViewport();
	/**
	 * @brief Searches for some kind of UObject in the world. Also 
	 * checks if there is only one instance of the object.
	 * 
	 */
	template <class T> T* FindUObject();

	UFUNCTION(BlueprintCallable, Category = "Getters")
	ADDPlayer* BlueprintGetPlayer();
	UFUNCTION(BlueprintCallable, Category = "Getters")
	ADDProjectileManager* BlueprintGetProjectileManager();

	ADDProjectileManager* ProjectileManager;
	ADDPlayer* Player;

	int32 TotalEnemiesKilled = 0;
	int32 TotalSouls = 0;
};