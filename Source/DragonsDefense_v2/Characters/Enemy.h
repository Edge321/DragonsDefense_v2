// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Characters/LivingActor.h"
#include "Enemy.generated.h"

class UStaticMeshComponent;
class UFloatingPawnMovement;
class UAudioComponent;

/**
 * 
 */
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile");
	FVector ProjectileOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI");
	float DistanceToAttack = 5.0f;
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
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector CheckDistance();
	void ApplyModifiers() override;
	void OnDeath() override;

	float TempShootCooldown;
};
