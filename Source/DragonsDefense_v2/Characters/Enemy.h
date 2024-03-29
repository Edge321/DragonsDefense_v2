// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "../Characters/LivingActor.h"
#include "Enemy.generated.h"

class UStaticMeshComponent;
class UFloatingPawnMovement;
class UMaterialBillboardComponent;
class UAudioComponent;
class ADDCastle;
class ADDPlayer;
class ADDProjectile;
class ADDPlaceable;
class UDDEnemyHealthBar;

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
	
	//insert AI component here if needed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh; //TODO - Replace with skeletal mesh when we get to animation
	//Utilized for attacking sentient placeables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* AttackSight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UFloatingPawnMovement* FloatingPawnMovement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UAudioComponent* ShootSound;

	//What projectile will the enemy be using?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile");
	TSubclassOf<ADDProjectile> Projectile;
	//Offset of spawned projectile relative to the origin of the enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile");
	FVector ProjectileOffset = FVector::Zero();
	//Distance from the castle to attack
	UPROPERTY(EditAnywhere, Category = "AI");
	float DistanceToAttack = 100.0f;
	//Distance from placeable to attack
	UPROPERTY(EditAnywhere, Category = "AI");
	float AttackSightDistance = 100.0f;
	UPROPERTY(EditAnywhere, Category = "AI");
	bool bEnableAttackPlaceables = true;
	UPROPERTY(EditAnywhere, Category = "Stats");
	float ShootCooldown = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Stats");
	float MovementSpeed = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Stats");
	float ProjectileSpeed = 800.0f;
	UPROPERTY(EditAnywhere, Category = "Stats");
	int32 SoulValue = 1;
	UPROPERTY(EditAnywhere, Category = "Stats");
	int32 Score = 1;
	UPROPERTY(BlueprintReadOnly, Category = "Temp")
	float TempHealth;
	
	//Variables below are all modifiers for Enemy's stats

	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	float EasyMovementSpeedMod = 0.9f;
	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	float HardMovementSpeedMod = 1.1f;
	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	float EasyShootCooldownMod = 1.2f;
	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	float HardShootCooldownMod = 0.8f;
	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	float EasyDamageMod = 0.8f;
	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	float HardDamageMod = 1.5f;
	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	int32 EasySoulValueMod = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Modifiers");
	int32 HardSoulValueMod = -1;

	UFUNCTION(BlueprintImplementableEvent, Category = "HealthBar")
	void UpdateHealthBar();

	void OnDeath() override;
	// Checks the distance of self and the castle
	void CheckDistance();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnConstruction(const FTransform& Transform) override;

	UStaticMeshComponent* GetMeshComponent() const;
	UFloatingPawnMovement* GetFloatingPawnMovement() const;
	const float GetMovementSpeed() const;
	const float GetDistanceFromCastle() const;
	const float GetMaxSpeed() const;

	void UpdateHealth(const float HealthModifier) override;

	FOnEnemyDeath OnEnemyDeath;

private:

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ApplyModifiers() override;
	void StartShooting();
	void StopShooting();
	void StartMoving();
	void StopMoving();
	void Shoot() const;
	void StartShootingPlaceable();
	void ShootPlaceable() const;
	// Checks if projectile was assigned to the enemy
	void ValidateProjectile();
	void FindPlayer();
	void AdjustAttackBox();

	ADDPlayer* Player;
	FTimerHandle ShootHandle;
	TArray<ADDPlaceable*> PlaceablesInSight;

	bool bIsShooting = false;
	float DistanceFromCastle;
};
