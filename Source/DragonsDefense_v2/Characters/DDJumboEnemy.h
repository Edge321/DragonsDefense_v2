// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Characters/Enemy.h"
#include "DDJumboEnemy.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API ADDJumboEnemy : public AEnemy
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "EnemiesToSpawn")
	TArray<TSubclassOf<AEnemy>> EnemyExplodies;

	void OnDeath() override;

private:
	
	void Explode();
};
