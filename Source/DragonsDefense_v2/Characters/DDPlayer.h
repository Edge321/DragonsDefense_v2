// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DDPlayer.generated.h"

class UCameraComponent;
class UFloatingPawnMovement;
class ADDProjectile;

UCLASS()
class DRAGONSDEFENSE_V2_API ADDPlayer : public APawn
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ADDProjectile> Projectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootCooldown = 0.6f;
	//Limits the area of where the player can move on the Y-axis
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AreaLimitY = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ProjectileOffset = FVector(0, 0, 0); //Where the projectile is spawned relative to the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool OnCooldown = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void ValidateProjectile();
	void ResetStats();
	/**
	 * @brief Limits area the player can move to
	 * 
	 */
	void LimitArea();

	UFUNCTION()
	void GameOverEventFunction();
	UFUNCTION()
	void GameStartEventFunction();
};
