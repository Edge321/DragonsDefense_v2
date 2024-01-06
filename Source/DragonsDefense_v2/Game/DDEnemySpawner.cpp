// Fill out your copyright notice in the Description page of Project Settings.


#include "DDEnemySpawner.h"
#include "Components/BillboardComponent.h"
//My classes
#include "../Game/DDGameModeBase.h"
#include "../Characters/Enemy.h"

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

	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->OnGameOver.AddDynamic(this, &ADDEnemySpawner::GameOverEventFunction);
		GameMode->OnGameStart.AddDynamic(this, &ADDEnemySpawner::GameStartEventFunction);
	}
}

void ADDEnemySpawner::SpawnEnemy()
{
	check(EnemiesToSpawn.Num() > 0)

	int32 EnemyIndex = FMath::RandRange(0, EnemiesToSpawn.Num() - 1);
	float RandomAreaY = FMath::RandRange(-SpawnAreaY, SpawnAreaY);

	FVector RandomLocation = FVector(GetActorLocation().X, RandomAreaY, GetActorLocation().Z);

	TSubclassOf<AEnemy> Enemy = EnemiesToSpawn[EnemyIndex];
	AEnemy* ActualEnemy = GetWorld()->SpawnActor<AEnemy>(Enemy, RandomLocation, GetActorRotation());

	if (ActualEnemy) {
		ActualEnemy->SpawnDefaultController();
		ActualEnemy->AutoPossessAI = EAutoPossessAI::Spawned;
		ActualEnemy->OnEnemyDeath.BindUObject(this, &ADDEnemySpawner::RemoveEnemyFromPool);
		AddEnemyToPool(ActualEnemy);
	}
}

void ADDEnemySpawner::AddEnemyToPool(AEnemy* Enemy)
{
	EnemyPool.Add(Enemy);
}

void ADDEnemySpawner::CleanPool()
{
	for (AEnemy* Enemy : EnemyPool) {
		if (Enemy) {
			Enemy->Destroy();
		}
	}
	EnemyPool.Empty();
}

void ADDEnemySpawner::GameOverEventFunction()
{
	StopSpawn();
	CleanPool();
}

void ADDEnemySpawner::GameStartEventFunction()
{
	StartSpawn();
}

void ADDEnemySpawner::RemoveEnemyFromPool(AEnemy* Enemy)
{
	int32 EnemyID = Enemy->GetUniqueID();

	for (AEnemy* SomeEnemy : EnemyPool) {
		if (SomeEnemy && SomeEnemy->GetUniqueID() == EnemyID) {
			SomeEnemy->Destroy();
			EnemyPool.Remove(SomeEnemy);
			break;
		}
	}
}

void ADDEnemySpawner::StartSpawn()
{
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ADDEnemySpawner::SpawnEnemy, SpawnInterval, true);
}

void ADDEnemySpawner::StopSpawn()
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}

