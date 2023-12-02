// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Characters/LivingActor.h"
#include "Enemy.generated.h"

class UStaticMeshComponent;
class UFloatingPawnMovement;
class UAudioComponent;
class ADDCastle;
class ADDProjectile;

DECLARE_DELEGATE_OneParam(FOnEnemyDeath, AEnemy*)

UCLASS()
class DRAGONSDEFENSE_V2_API AEnemy : public ALivingActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//COMPONENTS
	//insert AI component here if needed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh; //TODO - Replace with skeletal mesh when we get to animation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UFloatingPawnMovement* FloatingPawnMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* ShootSound;

	//VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement");
	float MovementSpeed = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile");
	TSubclassOf<ADDProjectile> Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile");
	FVector ProjectileOffset = FVector::Zero();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI");
	float DistanceToAttack = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI");
	float ShootCooldown = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers");
	float EasyMovementSpeedMod = 0.9f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers");
	float HardMovementSpeedMod = 1.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers");
	float EasyShootCooldownMod = 1.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers");
	float HardShootCooldownMod = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score");
	int32 Score = 1;

	//FUNCTIONS
	UFUNCTION(BlueprintNativeEvent)
	void OnDeath() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FOnEnemyDeath OnEnemyDeath;

private:
	void CheckDistance();
	void ApplyModifiers() override;
	void StartShooting();
	void StopShooting();
	void Shoot();
	void ValidateProjectile();
	void FindCastle();

	ADDCastle* Castle;
	FTimerHandle ShootHandle;
	float TempShootCooldown;
	bool IsShooting = false;
};
