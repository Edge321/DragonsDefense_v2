// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DDGameModeBase.generated.h"

// TODO - Add an enum class for difficulties

//DECLARE_MULTICAST_DELEGATE(FOnGameOver)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);

class UDDScoreWidget;

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API ADDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	
	virtual void BeginPlay() override;

	int32 TotalEnemiesKilled = 0;
	
	//Todo: Add widget stuff here
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UUserWidget> ScoreWidgetClass;

	UPROPERTY()
	UDDScoreWidget* ScoreWidget;

	void UpdateScoreText();

public:

	void AddScore(int32 Score);
	void AddToActorPool(AActor* Actor);
	void RemoveActorFromPool(AActor* Actor);
	void GameOver();

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnGameOver OnGameOver;

private:
	TArray<AActor*> ActorPool;
	void RemoveAllActors();

};
