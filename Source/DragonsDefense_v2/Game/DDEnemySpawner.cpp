// Fill out your copyright notice in the Description page of Project Settings.


#include "DDEnemySpawner.h"
#include "Components/BillboardComponent.h"

// Sets default values
ADDEnemySpawner::ADDEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnerIcon = CreateDefaultSubobject<UBillboardComponent>("SpawnerIcon");

	RootComponent = SpawnerIcon;
}

// Called when the game starts or when spawned
void ADDEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ADDEnemySpawner::SpawnEnemy, SpawnInterval, true);
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ADDEnemySpawner::StartSpawnTimer, SpawnInterval, true);
}

// Called every frame
void ADDEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADDEnemySpawner::SpawnEnemy()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Spawned an enemy!"));
	//GetWorld()->SpawnActor(Enemy, GetActorLocation());
}

void ADDEnemySpawner::StartSpawnTimer()
{

}

void ADDEnemySpawner::ClearSpawnTimer()
{

}

