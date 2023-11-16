// Fill out your copyright notice in the Description page of Project Settings.


#include "DDEnemySpawner.h"
#include "Components/BillboardComponent.h"
#include "../Game/DDGameModeBase.h"

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

	StartSpawn();

	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		//GameMode->OnGameOver.AddLambda([this]() {
			//this->GameOverEventFunction();
			//});
		GameMode->OnGameOver.AddDynamic(this, &ADDEnemySpawner::GameOverEventFunction);
	}
}

void ADDEnemySpawner::SpawnEnemy()
{
	if (EnemiesToSpawn.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("Error: EnemiesToSpawn array is empty!"))
	}

	int32 EnemyIndex = FMath::RandRange(0, EnemiesToSpawn.Num() - 1);
	float RandomAreaY = FMath::RandRange(-SpawnAreaY, SpawnAreaY);

	FVector RandomLocation = FVector(GetActorLocation().X, RandomAreaY, GetActorLocation().Z);

	TSubclassOf<AEnemy> Enemy = EnemiesToSpawn[EnemyIndex];
	AEnemy* ActualEnemy = GetWorld()->SpawnActor<AEnemy>(Enemy, RandomLocation, GetActorRotation());

	if (ActualEnemy) {
		//TODO - Figure out what to do with the returned pointer. Add to enemy pool perhaps?
		ActualEnemy->SpawnDefaultController();
		ActualEnemy->AutoPossessAI = EAutoPossessAI::Spawned;
	}
}

void ADDEnemySpawner::GameOverEventFunction()
{
	StopSpawn();
}

void ADDEnemySpawner::StartSpawn()
{
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ADDEnemySpawner::SpawnEnemy, SpawnInterval, true);
}

void ADDEnemySpawner::StopSpawn()
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}

