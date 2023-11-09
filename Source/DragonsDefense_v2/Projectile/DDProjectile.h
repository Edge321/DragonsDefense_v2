// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDProjectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class UAudioComponent;
class UNiagaraComponent;

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

	//COMPONENTS
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

	//VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage");
	float ProjectileDamage = -1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier");
	float EasyModifier = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier");
	float HardModifier = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lifetime");
	float ProjectileLifetime = 10.0f;

public:
	void SetVelocity(FVector Velocity);
	/*FVector GetVelocity();*/
	void SetProjectileOwner(uint32 ActorID);

private:
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	
	void ApplyModifiers();
	void EnableCollision();
	void DestroySelf();

	FTimerHandle ProjectileTimer;
	uint32 OwnerID;
};
