// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Game/DDGameModeBase.h"
#include "Components/BoxComponent.h"
#include "LivingActor.generated.h"

/**
 * An abstract class meant for actors that have health
 */
UCLASS(Abstract)
class DRAGONSDEFENSE_V2_API ALivingActor : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALivingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* Collider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health");
	float Health = 1.0f;
	//Modifies health if Easy mode was chosen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers");
	float EasyHealthModifier = 0.8f;
	//Modifies health if Hard mode was chosen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers");
	float HardHealthModifier = 1.2f;

	//Apparently Unreal does not like pure virtual functions so...yeah
	virtual void ApplyModifiers() {}
	virtual void OnDeath() {}

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual const float GetHealth();
	virtual void UpdateHealth(const float HealthModifier);
};
