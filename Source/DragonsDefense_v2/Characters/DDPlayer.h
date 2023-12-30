// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Characters/LivingActor.h"
#include "DDPlayer.generated.h"

class UCameraComponent;
class UFloatingPawnMovement;
class ADDProjectile;

UCLASS()
class DRAGONSDEFENSE_V2_API ADDPlayer : public ALivingActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADDPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh; //TODO - Later down the line change to SkeletalMeshComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UFloatingPawnMovement* FloatingPawnMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<ADDProjectile> Projectile;
	//Base movement speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MovementSpeed = 20.0f;
	//Base shooting speed (in seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float ShootSpeed = 0.6f;
	//Limits the area of where the player can move on the Y-axis
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Area Limit")
	float AreaLimitY = 50.0f;
	//Where the projectile is spawned relative to the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ProjectileOffset = FVector(0, 0, 0); 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool OnCooldown = false;

	
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	const float GetHealth() override;
	const float GetMaxHealth() const;
	
	void UpdateHealth(const float HealthModifier) override;
	void UpdateMaxHealth(const float MaxHealthModifier);
	void UpdateMovementSpeed(const float MovementSpeedModifier);
	void UpdateShootSpeed(const float ShootSpeedModifier);
	void UpdateArmor(const float ArmorModifier);

private:
	void ValidateProjectile();
	void ResetStats();
	/**
	 * @brief Limits area the player can move to
	 * 
	 */
	void LimitArea();
	void OnDeath() override;

	UFUNCTION()
	void GameOverEventFunction();
	UFUNCTION()
	void GameStartEventFunction();

	float MaxHealth;
	float TempHealth;
	float TempMovementSpeed;
	float TempShootSpeed;
	float Armor;
};
