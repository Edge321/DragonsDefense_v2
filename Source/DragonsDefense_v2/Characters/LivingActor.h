// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Game/DDGameModeBase.h"
#include "LivingActor.generated.h"

class UBoxComponent;

UCLASS(Abstract)
class DRAGONSDEFENSE_V2_API ALivingActor : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALivingActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetHealth();
	void SetHealth(float HealthModifier);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//COMPONENTS
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//UBoxComponent* Collider;
	///Needing colliders is a lie apparently?! only need the mesh i suppose...

	//VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health");
	float Health = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers");
	float EasyHealthModifier = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers");
	float HardHealthModifier = 1.2f;

	//VIRTUAL FUNCTIONS
	virtual void ApplyModifiers() {}
	virtual void OnDeath() {}
};
