// Fill out your copyright notice in the Description page of Project Settings.


#include "DDJumboEnemy.h"
//My classes
#include "../Game/DDGameModeBase.h"
#include "../Game/DDEnemySpawner.h"

void ADDJumboEnemy::OnDeath()
{
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->AddScore(Score);
		GameMode->UpdateSouls(SoulValue);
	}
	
	Explode();
	OnEnemyDeath.ExecuteIfBound(this);
}

void ADDJumboEnemy::Explode() const
{
	ADDEnemySpawner* Spawner;
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		Spawner = &(GameMode->GetEnemySpawner());
		int RandomSpawn = FMath::RandRange(LowEnemySpawn, HighEnemySpawn);
		Spawner->SpawnEnemies(EnemyExplodies, RandomSpawn, GetActorLocation());
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("How did we get here?"))
		return;
	}
}
