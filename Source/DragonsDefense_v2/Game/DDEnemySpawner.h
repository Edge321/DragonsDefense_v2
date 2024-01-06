// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDEnemySpawner.generated.h"

class UBillboardComponent;
class AEnemy;

/**
 * Does what the class's name says as well as
 * has a pool of enemies to manage
 */
UCLASS()
class DRAGONSDEFENSE_V2_API ADDEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* SpawnerIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	float SpawnInterval = 3.0f;
	//Area of where an enemy can be spawned on the Y-axis
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	float SpawnAreaY = 1500.0f;
	//Used for reducing the spawn interval for fast enemy spawning
	//Not currently used, but prob will be later on with waves implemented
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	float SpawnTimeReducer;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TArray<TSubclassOf<AEnemy>> EnemiesToSpawn;

private:
	void StartSpawn();
	void StopSpawn();
	void SpawnEnemy();
	void AddEnemyToPool(AEnemy* Enemy);
	void RemoveEnemyFromPool(AEnemy* Enemy);
	void CleanPool();

	UFUNCTION()
	void GameOverEventFunction();
	UFUNCTION()
	void GameStartEventFunction();

	FTimerHandle SpawnTimerHandle;
	TArray<AEnemy*> EnemyPool;
};
