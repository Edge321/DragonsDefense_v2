// Fill out your copyright notice in the Description page of Project Settings.


#include "DDPlaceableManager.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"
//My classes
#include "../Game/DDPlaceablePreview.h"
#include "../Game/DDPlaceablePurchaseInfo.h"
#include "../Game/DDGameModeBase.h"
#include "../Characters/DDPlaceable.h"

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
	
	if (Preview) {
		Preview->SetActorHiddenInGame(true);
		Preview->OnColliding.BindUObject(this, &ADDPlaceableManager::SetPreviewMaterial);
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("Something went wrong with the preview!"))
	}

	CheckPreviewValidity();
	InitializePurchaseInfo();

	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->OnGameOver.AddDynamic(this, &ADDPlaceableManager::GameOverEventFunction);
		GameMode->OnGameStart.AddDynamic(this, &ADDPlaceableManager::GameStartEventFunction);
		GameMode->OnWaveOver.AddDynamic(this, &ADDPlaceableManager::WaveOverEventFunction);
		GameMode->OnWaveStart.AddDynamic(this, &ADDPlaceableManager::WaveStartEventFunction);
		GameMode->OnPlacing.AddDynamic(this, &ADDPlaceableManager::SetPreviewStatus);
	}
}

void ADDPlaceableManager::ChangePreviewMesh(UStaticMesh* Mesh, const FVector Scale)
{
	Preview->SetMesh(Mesh);
	Preview->SetScale(Scale);
}

void ADDPlaceableManager::SetPreviewStatus(bool IsPlacing) {
	Preview->SetActorHiddenInGame(!IsPlacing);
}

bool ADDPlaceableManager::IsPreviewDisabled() const
{
	return Preview->IsHidden();
}

FVector ADDPlaceableManager::GetPreviewLocation() const
{
	return Preview->GetActorLocation();
}

void ADDPlaceableManager::CanPlace(bool PlaceStatus)
{
	bCanPlace = PlaceStatus;
}

//TODO - might not need this tbh, we will see
void ADDPlaceableManager::SpawnPlaceable(TSubclassOf<ADDPlaceable> PlaceableClass, const FVector Location, const FRotator Rotation)
{
	 ADDPlaceable* Placeable = GetWorld()->SpawnActor<ADDPlaceable>(PlaceableClass, Location, Rotation);

	if (Placeable) {
		Placeable->SpawnDefaultController();
		Placeable->AutoPossessAI = EAutoPossessAI::Spawned;
		Placeable->OnPlaceableDeath.BindUObject(this, &ADDPlaceableManager::RemovePlaceableFromPool);
		AddPlaceableToPool(Placeable);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Something went wrong with spawning a placeable!"))
	}
}

void ADDPlaceableManager::SetCurrentPlaceable(TSubclassOf<ADDPlaceable> PlaceableClass, int32 Price)
{
	//A bit of optimization
	if (CurrentPlaceableClass == PlaceableClass) {
		return;
	}

	CurrentPlaceableClass = PlaceableClass;

	//Not sure if this is the greatest way to get the scale of the actor
	ADDPlaceable* Placeable = GetWorld()->SpawnActor<ADDPlaceable>(PlaceableClass);

	if (Placeable) {
		FVector Scale = Placeable->GetActorScale();
		UStaticMesh* Mesh = Placeable->GetMesh()->GetStaticMesh();
		ChangePreviewMesh(Mesh, Scale);
		PlaceableInfo.SetPrice(Price);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Something went wrong with setting the preview object"))
	}

	Placeable->Destroy();
}

void ADDPlaceableManager::SpawnPlaceableAtCursor(TSubclassOf<ADDPlaceable> PlaceableClass)
{
	ADDPlaceable* Placeable = GetWorld()->SpawnActor<ADDPlaceable>(PlaceableClass, GetPreviewLocation(), Preview->GetActorRotation());
	//BUG -  Placing placeable inside anything with collision will result in placeable being destroyed before being put in pool
	// results in a memory violation error
	if (Placeable) {
		Placeable->SpawnDefaultController();
		Placeable->AutoPossessAI = EAutoPossessAI::Spawned;
		Placeable->OnPlaceableDeath.BindUObject(this, &ADDPlaceableManager::RemovePlaceableFromPool);
		AddPlaceableToPool(Placeable);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Something went wrong with spawning a placeable at the cursor!"))
	}
}

void ADDPlaceableManager::PurchasePlaceableAtCursor()
{
	if (PlaceableInfo.IsBuyable() && bCanPlace) {
		PlaceableInfo.UpdateSouls();

		ADDPlaceable* Placeable = GetWorld()->SpawnActor<ADDPlaceable>(CurrentPlaceableClass, GetPreviewLocation(), Preview->GetActorRotation());
		//BUG -  Placing placeable inside anything with collision will result in placeable being destroyed before being put in pool
		// results in a memory violation error
		if (Placeable) {
			Placeable->SpawnDefaultController();
			Placeable->AutoPossessAI = EAutoPossessAI::Spawned;
			Placeable->OnPlaceableDeath.BindUObject(this, &ADDPlaceableManager::RemovePlaceableFromPool);
			AddPlaceableToPool(Placeable);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Something went wrong with spawning a placeable at the cursor!"))
		}
	}
}

void ADDPlaceableManager::CheckPreviewValidity()
{
	check(Preview)
}

void ADDPlaceableManager::AddPlaceableToPool(ADDPlaceable* Placeable)
{
	PlaceablePool.Add(Placeable);
}

void ADDPlaceableManager::RemovePlaceableFromPool(ADDPlaceable* Placeable)
{
	//TODO - Could possibly use maps or whatever UE uses for fast access
	int32 UniqueID = Placeable->GetUniqueID();

	for (ADDPlaceable* SomePlaceable : PlaceablePool) {
		if (SomePlaceable && SomePlaceable->GetUniqueID() == UniqueID) {
			SomePlaceable->Destroy();
			PlaceablePool.Remove(SomePlaceable);
			break;
		}
	}
}

void ADDPlaceableManager::ClearPool()
{
	for (ADDPlaceable* Placeable : PlaceablePool) {
		if (Placeable) {
			Placeable->Destroy();
		}
	}
	PlaceablePool.Empty();
}

void ADDPlaceableManager::InitializePurchaseInfo()
{
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());

	if (GameMode) {
		PlaceableInfo.SetGameMode(GameMode);
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("How did we get here?"))
	}
}

void ADDPlaceableManager::GameOverEventFunction()
{
	ClearPool();
}

void ADDPlaceableManager::GameStartEventFunction()
{
	//TODO - think about how necessary this is
}

void ADDPlaceableManager::WaveStartEventFunction()
{
	//TODO - think about how necessary this is
}

void ADDPlaceableManager::WaveOverEventFunction()
{
	//TODO - think about how necessary this is
}
