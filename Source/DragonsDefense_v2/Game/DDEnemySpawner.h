// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Characters/Enemy.h"
#include "DDEnemySpawner.generated.h"

class UBillboardComponent;

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
	float SpawnArea;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnTimeReducer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<TSubclassOf<AEnemy>> EnemiesToSpawn;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartSpawn();
	void StopSpawn();

private:
	void SpawnEnemy();

	FTimerHandle SpawnTimerHandle;
};
