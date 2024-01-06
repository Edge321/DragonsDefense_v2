// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DDGameModeBase.generated.h"

class UDDScoreWidget;
class UDDMainMenuWidget;
class UDDSoulShopWidget;
class ADDProjectileManager;
class ADDPlayer;
enum class EDifficulty : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateSouls);

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

	//So we have an array of widget classes. Wtf do I do for array of
	//various sorts of widgets like below?
	//TODO - Are these UPROPERTY necessary?
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
	
	UFUNCTION(BlueprintPure, Category = "Getters")
	const EDifficulty GetDifficulty() const;
	UFUNCTION(BlueprintCallable, Category = "Setters")
	void SetDifficulty(const EDifficulty NewDifficulty);
	UFUNCTION(BlueprintPure, Category = "Getters")
	const int32 GetSouls() const;
	UFUNCTION(BlueprintPure, Category = "Getters")
	UDDMainMenuWidget* GetMainMenuWidget() const;
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

	//Blueprint functions below are so C++ can't call it but blueprints can
	//since C++ can modify the pointers (even though you can change by reference
	//to pointer but shut up okay?)

	UFUNCTION(BlueprintCallable, Category = "Getters")
	ADDPlayer* BlueprintGetPlayer();
	UFUNCTION(BlueprintCallable, Category = "Getters")
	ADDProjectileManager* BlueprintGetProjectileManager();

	ADDProjectileManager* ProjectileManager;
	ADDPlayer* Player;

	EDifficulty Difficulty;

	int32 TotalEnemiesKilled = 0;
	int32 TotalSouls = 0;
};