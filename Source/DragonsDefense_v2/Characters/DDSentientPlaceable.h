// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Characters/DDPlaceable.h"
#include "DDSentientPlaceable.generated.h"

class USphereComponent;
class AEnemy;

enum class PlaceableAI : uint8;

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API ADDSentientPlaceable : public ADDPlaceable
{
	GENERATED_BODY()
	
public:

	ADDSentientPlaceable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//TODO - Wayyy later down the line, replace this with a skeletal mesh for animations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* AttackCollider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float AttackRadius = 25.0f;
	//How fast Placeable attacks in seconds
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float AttackSpeed = 1.0f;

public:

	void OnConstruction(const FTransform& Transform) override;
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetAI(const PlaceableAI AIState);
	
	const UStaticMeshComponent* GetMesh() const;

private:

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Attack() const;
	void Deactive();
	void AddEnemy(AEnemy* Enemy);
	void RemoveEnemy(AEnemy* Enemy);

	PlaceableAI CurrentAI;
	TArray<AEnemy*> EnemiesInArea;

};
