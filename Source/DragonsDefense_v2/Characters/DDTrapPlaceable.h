// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Characters/DDPlaceable.h"
#include "DDTrapPlaceable.generated.h"

/**
 * 
 */
UCLASS()
class DRAGONSDEFENSE_V2_API ADDTrapPlaceable : public ADDPlaceable
{
	GENERATED_BODY()
	
public:

	ADDTrapPlaceable();


protected:

	virtual void BeginPlay() override;

	const UStaticMeshComponent* GetMesh() const override;
	void EnableHighlight() override;
	void DisableHighlight() override;
	void OnSpawnOverlap(TArray<AActor*> OverlapActors) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles")
	bool bIgnoreProjectiles = false;

private:

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
