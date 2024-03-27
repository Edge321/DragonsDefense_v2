// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Game/DDPlaceablePurchaseInfo.h"
#include "DDPlaceableManager.generated.h"

class ADDPlaceablePreview;
class ADDPlaceable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKilledPlaceable, ADDPlaceable*, Placeable);

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
	bool IsPreviewDisabled() const;
	UFUNCTION(BlueprintCallable, Category = "PlaceablePreview")
	FVector GetPreviewLocation() const;
	UFUNCTION(BlueprintCallable, Category = "PlaceablePreview")
	void CanPlace(bool PlaceStatus);
	//Checks if preview should be in red or green due to certain factors
	UFUNCTION(BlueprintImplementableEvent, Category = "PlaceablePreview")
	void IsPreviewValid();
	UFUNCTION(BlueprintPure, Category = "PlaceableInfo")
	bool IsBuyable() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* ManagerIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlaceablePreview")
	TSubclassOf<ADDPlaceablePreview> PreviewClass;
	UPROPERTY(BlueprintReadOnly, Category = "PlaceablePreview")
	ADDPlaceablePreview* Preview;
	//Array for placeables that will be spawned at the beginning of the game
	//Needed for placeable info to fetch placeables' data properly
	UPROPERTY(EditDefaultsOnly, Category = "Placeable")
	TArray<TSubclassOf<ADDPlaceable>> PlaceablesToSpawn;
	UPROPERTY(BlueprintReadOnly, Category = "PlaceablePreview")
	bool bCanPlace = true;
	UPROPERTY(BlueprintReadOnly, Category = "PlaceablePrice")
	int32 CurrentPlaceablePrice = 0;

public:

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnPlaceable(TSubclassOf<ADDPlaceable> PlaceableClass, const FVector Location, const FRotator Rotation);
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnPlaceableAtCursor(TSubclassOf<ADDPlaceable> PlaceableClass);
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void PurchasePlaceableAtCursor();
	UFUNCTION(BlueprintCallable, Category = "PlaceablePreview")
	void SetCurrentPlaceable(TSubclassOf<ADDPlaceable> PlaceableClass, int32 Price);

	UPROPERTY(BlueprintAssignable)
	FOnKilledPlaceable OnKilledPlaceable;

private:

	void InitializePurchaseInfo();
	void CheckPreviewValidity();
	void AddPlaceableToPool(ADDPlaceable* Placeable);
	void RemovePlaceableFromPool(ADDPlaceable* Placeable);
	void ClearPool();
	void ChangePreviewMesh(UStaticMesh* Mesh, const FVector Scale);

	UFUNCTION()
	void GameOverEventFunction();
	UFUNCTION()
	void SetPreviewStatus(bool IsPlacing);

	TSubclassOf<ADDPlaceable> CurrentPlaceableClass;
	TArray<ADDPlaceable*> PlaceablePool;
	
	FDDPlaceablePurchaseInfo PlaceableInfo;
};
