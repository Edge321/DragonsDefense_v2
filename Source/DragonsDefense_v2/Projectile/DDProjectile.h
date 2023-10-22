// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDProjectile.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UProjectileMovementComponent;

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

	//VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float ProjectileDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float easyModifier = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float hardModifier = 2.0f;
};
