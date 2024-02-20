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

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	APlayerController* Controller;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const FVector GetMeshSize() const;
	void SetMesh(UStaticMesh* NewMesh);

private:

	void GetControllerReference();
	void DisplayPreview();

	

};
