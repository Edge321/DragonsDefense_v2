// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDPlaceablePreview.generated.h"

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetMesh(UStaticMesh* NewMesh);
	void SetScale(FVector Scale);

private:

	APlayerController* Controller;

	const FVector GetMeshSize() const;
	void GetControllerReference();
	void UpdatePreview();

};
