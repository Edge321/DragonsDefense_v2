// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDEnemySpawner.generated.h"

class UBillboardComponent;
class AEnemy;

USTRUCT(BlueprintType)
struct FWaveInfo {

	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Waves")
	TArray<TSubclassOf<AActor>> EnemiesToSpawn;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Waves")
	int32 WaveTrigger = 1;
	//Total enemies to spawn in the wave
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Waves")
	int32 NumberOfEnemies = 10;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Waves")
	float LowSpawnInterval = 3.0f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Waves")
	float HighSpawnInterval = 5.0f;
	//Sets which wave to be called first. Higher number = First wave to be called
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Waves")
	int32 Priority = 1;

};

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
	UPROPERTY(EditDefaultsOnly, Category = "Waves")
	TArray<FWaveInfo> Waves;

public:
	//Pretty much a special function that spawns at a location 
	//and causes enemies to burst from the passed in location
	void SpawnEnemies(TArray<TSubclassOf<AEnemy>> Enemies, int32 EnemyAmount, FVector SpawnLocation);

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
	int32 EnemyCounter = 0;
	int32 CurrentWave = 1;
};
