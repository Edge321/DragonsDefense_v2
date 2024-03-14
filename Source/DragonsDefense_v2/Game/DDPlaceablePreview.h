// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDPlaceablePreview.generated.h"

DECLARE_DELEGATE(FOnColliding);

UCLASS()
class DRAGONSDEFENSE_V2_API ADDPlaceablePreview : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDPlaceablePreview();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* Collider;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* OptionalRadiusMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Material")
	void SetMaterial(UMaterialInstance* Material);
	UFUNCTION(BlueprintCallable, Category = "Material")
	const bool GetCurrentlyColliding() const;

	void SetMesh(UStaticMesh* NewMesh);
	void SetScale(FVector Scale);
	void SetRadiusSize(const float AttackRadius, const float RadiusMeshDiameter);
	void EnableAttackRadius();
	void DisableAttackRadius();
	void ClearActorsArray();
	void CalculateOffset();


	FOnColliding OnColliding;

private:

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void RemoveActor(AActor* Actor);
	void AddActor(AActor* Actor);
	const FVector GetMeshSize() const;
	void GetControllerReference();
	void UpdatePreview();

	TArray<AActor*> ActorsColliding;
	FVector AdjustedOffset = FVector::Zero();

	APlayerController* Controller;
};
