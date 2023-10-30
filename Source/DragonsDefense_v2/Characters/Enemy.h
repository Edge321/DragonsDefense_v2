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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile");
	FVector ProjectileOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI");
	float DistanceToAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI");
	float ShootCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers");
	float EasyMovementSpeedMod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers");
	float HardMovementSpeedMod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers");
	float EasyShootCooldownMod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers");
	float HardShootCooldownMod;

	//FUNCTIONS
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) override;
	
private:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector CheckDistance();
	void ApplyModifiers() override;
	void OnDeath() override;
};
