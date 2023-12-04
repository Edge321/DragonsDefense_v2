// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDProjectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class UAudioComponent;
class UNiagaraComponent;

DECLARE_DELEGATE_OneParam(FOnProjectileDestroyed, ADDProjectile*)

/**
 * The Projectile class is used to damage any LivingActor in the world
 */
UCLASS()
class DRAGONSDEFENSE_V2_API ADDProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* Collider;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* ImpactSound;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* ProjectileEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage");
	float ProjectileDamage = -1.0f;
	//Modifies projectile's stats if Easy mode is chosen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier");
	float EasyModifier = 0.5f;
	//Modifies projectile's stats if Hard mode is chosen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier");
	float HardModifier = 2.0f;
	//Length of life in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifetime");
	float ProjectileLifetime = 10.0f;

public:
	void SetVelocity(FVector Velocity);
	/**
	 * @brief Who owns the projectile
	 * 
	 * @param ActorID ID of the UObject owner
	 */
	void SetProjectileOwner(uint32 ActorID);
	void SetCollisionChannelToIgnore(ECollisionChannel Channel);

	FOnProjectileDestroyed OnProjectileDestroyed;

private:
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	
	/**
	 * @brief Applies the necessary modifiers to any of the projectile's stats
	 * 
	 */
	void ApplyModifiers();
	void EnableCollision();
	void DestroySelf();

	FTimerHandle ProjectileTimer;
	//ID of projectile's owner
	uint32 OwnerID;
};
