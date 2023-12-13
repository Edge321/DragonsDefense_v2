// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Characters/LivingActor.h"
#include "DDCastle.generated.h"

class UStaticMeshComponent;
class ADDPlayer;

UCLASS()
class DRAGONSDEFENSE_V2_API ADDCastle : public ALivingActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDCastle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	//VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float ArmorHealth = 0;

	void UpdateHealth(const float HealthModifier) override;
	const float GetHealth() override;

private:
	void SetMaxHealth(const float MaxHealthModifier);
	const float GetMaxHealth();
	void ResetStats();
	/**
	 * @brief Applies difficulty modifier to the necessary
	 * stats
	 * 
	 */
	void ApplyModifiers() override;
	void OnDeath() override;
	void GameOverEventFunction();
	void GameStartEventFunction();

	float TempHealth;
	float MaxHealth;

};
