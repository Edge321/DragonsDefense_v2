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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* Collider;


	//VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement");
	float MovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health");
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers");
	float EasyHealthModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers");
	float HardHealthModifier;


	float TempHealth;

	//VIRTUAL FUNCTIONS
	virtual void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) {}
	virtual void ApplyModifiers() {}
	virtual void OnDeath() {}

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetHealth();
	void SetHealth(float HealthModifier);
};
