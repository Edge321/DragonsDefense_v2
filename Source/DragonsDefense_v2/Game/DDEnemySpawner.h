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

	UPROPERTY(EditDefaultsOnly, Category = "Waves")
	TArray<TSubclassOf<AEnemy>> EnemiesToSpawn;
	UPROPERTY(EditDefaultsOnly, Category = "Waves")
	int32 WaveTrigger = 1;
	//Total enemies to spawn in the wave
	UPROPERTY(EditDefaultsOnly, Category = "Waves")
	int32 NumberOfEnemies = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Waves")
	float LowSpawnInterval = 3.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Waves")
	float HighSpawnInterval = 5.0f;
	//Sets which wave to be called first. Higher number = First wave to be called
	UPROPERTY(EditDefaultsOnly, Category = "Waves")
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

	
	//Area of where an enemy can be spawned on the Y-axis
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	float SpawnAreaY = 1500.0f;
	//Used for reducing the spawn interval for fast enemy spawning
	//Not currently used, but maybe will be later on with waves implemented
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	float SpawnTimeReducer;
	UPROPERTY(EditDefaultsOnly, Category = "Waves")
	TArray<FWaveInfo> Waves;

public:
	//Pretty much a special function that spawns at a location 
	//and causes enemies to burst from the passed in location
	void SpawnEnemies(const TArray<TSubclassOf<AEnemy>> Enemies, const int32 EnemyAmount, const FVector SpawnLocation);

	//Functions below are for The Admin pretty much

	void SetEnemiesToSpawn(const TArray<TSubclassOf<AEnemy>> NewEnemiesToSpawn);
	void SetMaxEnemySpawn(const int32 NewMaxEnemySpawn);
	void SetSpawnInterval(const float NewSpawnInterval);
	//TODO -  Add low and high spawn intervals

	TArray<TSubclassOf<AEnemy>> GetEnemiesToSpawn() const;
	int32 GetMaxEnemySpawn() const;
	float GetSpawnInterval() const;

private:
	void StartSpawn();
	void StopSpawn();
	void SpawnEnemy();
	void AddEnemyToPool(AEnemy* Enemy);
	void RemoveEnemyFromPool(AEnemy* Enemy);
	void CleanPool();
	void SortWaves();
	FWaveInfo SearchWave() const;
	void SetWaveStats(const FWaveInfo Wave);
	void CheckEnemiesSpawned();
	void CheckIfWaveOver() const;

	UFUNCTION()
	void GameOverEventFunction();
	UFUNCTION()
	void GameStartEventFunction();
	UFUNCTION()
	void WaveStartEventFunction();
	UFUNCTION()
	void WaveOverEventFunction();

	TArray<TSubclassOf<AEnemy>> EnemiesToSpawn;
	float SpawnInterval = 3.0f;

	float OriginalSpawnInterval;
	FTimerHandle SpawnTimerHandle;
	TArray<AEnemy*> EnemyPool;

	int32 MaxEnemySpawn = 1;
	int32 EnemyCounter = 0;
	int32 CurrentWave = 1;
};
