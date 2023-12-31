// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<ADDProjectile> Projectile;
	//Where the projectile is spawned relative to the player's center
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	FVector ProjectileOffset = FVector(0, 0, 0); 
	//Base movement speed
	UPROPERTY(EditAnywhere, Category = "Stats")
	float MovementSpeed = 20.0f;
	//Base shooting speed (in seconds)
	UPROPERTY(EditAnywhere, Category = "Stats")
	float ShootSpeed = 0.6f;
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float TempMovementSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float TempDamage;
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	float TempShootSpeed;
	//Limits the area of where the player can move on the Y-axis
	UPROPERTY(EditAnywhere, Category = "Area Limit")
	float AreaLimitY = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooldown")
	bool OnCooldown = false;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintPure, Category = "Getters")
	const float GetHealth() const override;
	UFUNCTION(BlueprintPure, Category = "Getters")
	const float GetMaxHealth() const;
	UFUNCTION(BlueprintPure, Category = "Getters")
	const float GetMovementSpeed() const;
	UFUNCTION(BlueprintPure, Category = "Getters")
	const float GetShootSpeed() const;
	UFUNCTION(BlueprintPure, Category = "Getters")
	const float GetArmor() const;
	UFUNCTION(BlueprintPure, Category = "Getters")
	const float GetDamage() const override;

	void UpdateHealth(const float HealthModifier) override;
	void UpdateMaxHealth(const float MaxHealthModifier);
	void UpdateMovementSpeed(const float MovementSpeedModifier);
	void UpdateShootSpeed(const float ShootSpeedModifier);
	void UpdateArmor(const float ArmorModifier);
	void UpdateDamage(const float DamageModifier);

private:
	void ValidateProjectile();
	void ResetStats();
	/**
	 * @brief Limits area the player can move in
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
	float Armor;
};
