// Fill out your copyright notice in the Description page of Project Settings.


#include "DDPlaceableManager.h"
#include "Components/BillboardComponent.h"
//My classes
#include "../Game/DDPlaceablePreview.h"

// Sets default values
ADDPlaceableManager::ADDPlaceableManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ManagerIcon = CreateDefaultSubobject<UBillboardComponent>("ManagerIcon");

	RootComponent = ManagerIcon;
}

// Called when the game starts or when spawned
void ADDPlaceableManager::BeginPlay()
{
	Super::BeginPlay();

	Preview = Cast<ADDPlaceablePreview>(GetWorld()->SpawnActor(PreviewClass));

	CheckPreviewValidity();
	
	//EnablePreview();
	//DisablePreview();
}

void ADDPlaceableManager::ChangePreviewMesh(UStaticMesh* Mesh)
{
	Preview->SetMesh(Mesh);
}

void ADDPlaceableManager::EnablePreview()
{
	Preview->SetActorHiddenInGame(true);
}

void ADDPlaceableManager::DisablePreview()
{
	Preview->SetActorHiddenInGame(false);
}

bool ADDPlaceableManager::IsPreviewDisabled() const
{
	return Preview->IsHidden();
}

void ADDPlaceableManager::CheckPreviewValidity()
{
	check(Preview)
}
