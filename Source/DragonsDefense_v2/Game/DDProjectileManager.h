// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDProjectileManager.generated.h"

class UBillboardComponent;
class ADDProjectile;

UCLASS()
class DRAGONSDEFENSE_V2_API ADDProjectileManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADDProjectileManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* ManagerIcon;

public:
	void AddProjectileToPool(ADDProjectile* Proj);

private:
	TArray<ADDProjectile*> ProjectilePool;

	void RemoveProjectileFromPool(ADDProjectile* Proj);
	void CleanPool();

	UFUNCTION()
	void GameOverEventFunction();
};
