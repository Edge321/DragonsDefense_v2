// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DDPlayer.generated.h"

class UCameraComponent;
class UFloatingPawnMovement;

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

	// COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UFloatingPawnMovement* FloatingPawnMovement;
	// VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float MovementSpeed = 20.0f;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
