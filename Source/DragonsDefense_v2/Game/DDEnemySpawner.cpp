// Fill out your copyright notice in the Description page of Project Settings.


#include "DDEnemySpawner.h"
#include "Components/BillboardComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
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

	OriginalSpawnInterval = SpawnInterval;

	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->OnGameOver.AddDynamic(this, &ADDEnemySpawner::GameOverEventFunction);
		GameMode->OnGameStart.AddDynamic(this, &ADDEnemySpawner::GameStartEventFunction);
		GameMode->OnWaveOver.AddDynamic(this, &ADDEnemySpawner::WaveOverEventFunction);
		GameMode->OnWaveStart.AddDynamic(this, &ADDEnemySpawner::WaveStartEventFunction);
	}

	SortWaves();
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

		EnemyCounter++;
	}

	CheckEnemiesSpawned();
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

void ADDEnemySpawner::SortWaves()
{
	//Fucked up world to be using lambda functions
	//Uses lambda function for a custom sorting (using Priority)
	Waves.Sort([&](const FWaveInfo& A, const FWaveInfo& B) {
		return A.Priority > B.Priority;
		});
}

FWaveInfo ADDEnemySpawner::SearchWave() const
{
	for (FWaveInfo Wave : Waves) {
		if (CurrentWave % Wave.WaveTrigger == 0) {
			return Wave;
		}
	}

	//Might as well always return the bottom of the barrel
	return Waves[Waves.Num() - 1];
}

void ADDEnemySpawner::SetWaveStats(const FWaveInfo Wave)
{
	EnemiesToSpawn = Wave.EnemiesToSpawn;
	MaxEnemySpawn = Wave.NumberOfEnemies;
	//TODO - Implement high and low spawn intervals
	SpawnInterval = Wave.LowSpawnInterval;
}

void ADDEnemySpawner::CheckEnemiesSpawned()
{
	if (EnemyCounter >= MaxEnemySpawn) {
		StopSpawn();
	}
}

void ADDEnemySpawner::CheckIfWaveOver() const
{
	if (EnemyCounter >= MaxEnemySpawn && EnemyPool.Num() <= 0) {
		ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode) {
			GameMode->WaveOver();
		}
	}
}

void ADDEnemySpawner::GameOverEventFunction()
{
	CurrentWave = 1;
	SpawnInterval = OriginalSpawnInterval; //Might not be necessary tbh
	EnemyCounter = 0;
	StopSpawn();
	CleanPool();
}

void ADDEnemySpawner::GameStartEventFunction()
{
	WaveStartEventFunction();
}

void ADDEnemySpawner::WaveStartEventFunction()
{
	UE_LOG(LogTemp, Log, TEXT("Current Wave: %d"), CurrentWave);
	FWaveInfo Wave = SearchWave();
	SetWaveStats(Wave);
	//The Admin would like to have a word in this function. Call him
	StartSpawn();
}

void ADDEnemySpawner::WaveOverEventFunction()
{
	EnemyCounter = 0;
	CurrentWave++;
	StopSpawn();
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

	CheckIfWaveOver();
}

void ADDEnemySpawner::SpawnEnemies(const TArray<TSubclassOf<AEnemy>> Enemies, const int32 EnemyAmount, const FVector SpawnLocation)
{
	if (Enemies.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Oy dumbass you passed in an empty array to EnemySpawner"))
		return;
	}

	int32 EnemyIndex;
	float RandomX;
	float RandomY;
	FVector RandomLocation;
	FVector PushForce;

	for (int i = 0; i < EnemyAmount; i++) {
		EnemyIndex = FMath::RandRange(0, Enemies.Num() - 1);
		int32 RandomSignX = FMath::RandBool() ? 1 : -1;
		int32 RandomSignY = FMath::RandBool() ? 1 : -1;
		RandomX = FMath::RandRange(300.0f, 500.0f) * RandomSignX;
		RandomY = FMath::RandRange(1000.0f, 3000.0f) * RandomSignY;

		TSubclassOf<AEnemy> Enemy = Enemies[EnemyIndex];
		AEnemy* ActualEnemy = GetWorld()->SpawnActor<AEnemy>(Enemy, SpawnLocation, GetActorRotation());

		PushForce = FVector(RandomX, RandomY, 0);

		if (ActualEnemy) {
			ActualEnemy->SpawnDefaultController();
			ActualEnemy->AutoPossessAI = EAutoPossessAI::Spawned;
			ActualEnemy->OnEnemyDeath.BindUObject(this, &ADDEnemySpawner::RemoveEnemyFromPool);
			ActualEnemy->GetFloatingPawnMovement()->Velocity += PushForce;
			AddEnemyToPool(ActualEnemy);
		}
	}
}

void ADDEnemySpawner::SetEnemiesToSpawn(const TArray<TSubclassOf<AEnemy>> NewEnemiesToSpawn)
{
	EnemiesToSpawn = NewEnemiesToSpawn;
}

void ADDEnemySpawner::SetMaxEnemySpawn(const int32 NewMaxEnemySpawn)
{
	MaxEnemySpawn = NewMaxEnemySpawn;
}

void ADDEnemySpawner::SetSpawnInterval(const float NewSpawnInterval)
{
	SpawnInterval = NewSpawnInterval;
}

TArray<TSubclassOf<AEnemy>> ADDEnemySpawner::GetEnemiesToSpawn() const
{
	return EnemiesToSpawn;
}

int32 ADDEnemySpawner::GetMaxEnemySpawn() const
{
	return MaxEnemySpawn;
}

float ADDEnemySpawner::GetSpawnInterval() const
{
	return SpawnInterval;
}

void ADDEnemySpawner::StartSpawn()
{
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ADDEnemySpawner::SpawnEnemy, SpawnInterval, true);
}

void ADDEnemySpawner::StopSpawn()
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}

