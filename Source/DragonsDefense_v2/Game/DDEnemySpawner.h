// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDEnemySpawner.generated.h"

class UBillboardComponent;
class AEnemy;

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

	//COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* SpawnerIcon;

	//VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnInterval = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnAreaY = 1500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnTimeReducer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<TSubclassOf<AEnemy>> EnemiesToSpawn;

private:
	void StartSpawn();
	void StopSpawn();
	void SpawnEnemy();
	void AddEnemyToPool(AEnemy* Enemy);
	void RemoveEnemyFromPool(AEnemy* Enemy);
	void CleanPool();

	void EnemyDeathEventFunction(AEnemy* Enemy);

	UFUNCTION()
	void GameOverEventFunction();
	UFUNCTION()
	void GameStartEventFunction();

	FTimerHandle SpawnTimerHandle;
	TArray<AEnemy*> EnemyPool;

	//TODO - Consider having a Pool for the spawner to avoid coupling (Enemy - GameMode)
};
