// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DDPlaceableManager.generated.h"

class ADDPlaceablePreview;

UCLASS()
class DRAGONSDEFENSE_V2_API ADDPlaceableManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDPlaceableManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "PlaceablePreview")
	void ChangePreviewMesh(UStaticMesh* Mesh);
	UFUNCTION(BlueprintCallable, Category = "PlaceablePreview")
	void EnablePreview();
	UFUNCTION(BlueprintCallable, Category = "PlaceablePreview")
	void DisablePreview();
	UFUNCTION(BlueprintCallable, Category = "PlaceablePreview")
	bool IsPreviewDisabled() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* ManagerIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Variables")
	TSubclassOf<ADDPlaceablePreview> PreviewClass;

private:
	void CheckPreviewValidity();

	ADDPlaceablePreview* Preview;
};
