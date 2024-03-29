// Fill out your copyright notice in the Description page of Project Settings.


#include "DDPlaceableManager.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"
//My classes
#include "../Game/DDPlaceablePreview.h"
#include "../Game/DDPlaceablePurchaseInfo.h"
#include "../Game/DDGameModeBase.h"
#include "../Characters/DDPlaceable.h"
#include "../Characters/DDSentientPlaceable.h"

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
		Preview->OnColliding.BindUObject(this, &ADDPlaceableManager::IsPreviewValid);
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("Something went wrong with the preview!"))
	}

	CheckPreviewValidity();
	InitializePurchaseInfo();

	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->OnGameOver.AddDynamic(this, &ADDPlaceableManager::GameOverEventFunction);
		GameMode->OnPlacing.AddDynamic(this, &ADDPlaceableManager::SetPreviewStatus);
		GameMode->OnUpdateSouls.AddDynamic(this, &ADDPlaceableManager::IsPreviewValid);
	}

	//Needed for placeable info to fetch data properly
	for (TSubclassOf<ADDPlaceable> Place : PlaceablesToSpawn) {
		SpawnPlaceable(Place, FVector(10000, 10000, 0), FRotator::ZeroRotator);
	}
}

void ADDPlaceableManager::ChangePreviewMesh(UStaticMesh* Mesh, const FVector Scale)
{
	Preview->SetMesh(Mesh);
	Preview->SetScale(Scale);
	Preview->CalculateOffset();
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

bool ADDPlaceableManager::IsBuyable() const
{
	return PlaceableInfo.IsBuyable();
}

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
	ADDPlaceable* Placeable = GetWorld()->SpawnActor<ADDPlaceable>(PlaceableClass, GetActorLocation(), GetActorRotation());
	Placeable->SetActorEnableCollision(false);
	
	if (Placeable) {
		FVector Scale = Placeable->GetActorScale();
		UStaticMesh* Mesh = Placeable->GetMesh()->GetStaticMesh();
		ChangePreviewMesh(Mesh, Scale);
		PlaceableInfo.SetPrice(Price);
		//Sentient properties here
		if (Placeable->IsA<ADDSentientPlaceable>()) {
			ADDSentientPlaceable* Sentient = Cast<ADDSentientPlaceable>(Placeable);
			float Radius = Sentient->GetAttackRadius();
			FVector Size = Sentient->GetRadiusMeshSize();
			//Only need Y for diameter
			Preview->SetRadiusSize(Radius, Size.Y);
			Preview->EnableAttackRadius();
		}
		else {
			Preview->DisableAttackRadius();
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Something went wrong with setting the preview object"))
	}

	Placeable->Destroy();
	IsPreviewValid();
}

void ADDPlaceableManager::SpawnPlaceableAtCursor(TSubclassOf<ADDPlaceable> PlaceableClass)
{
	ADDPlaceable* Placeable = GetWorld()->SpawnActor<ADDPlaceable>(PlaceableClass, GetPreviewLocation(), Preview->GetActorRotation());
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
		if (Placeable) {
			Placeable->SpawnDefaultController();
			Placeable->AutoPossessAI = EAutoPossessAI::Spawned;
			Placeable->OnPlaceableDeath.BindUObject(this, &ADDPlaceableManager::RemovePlaceableFromPool);
			Placeable->SetSellingPrice(PlaceableInfo.GetPrice());
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
	OnKilledPlaceable.Broadcast(Placeable);

	ADDPlaceable* ToRemove = nullptr;

	for (ADDPlaceable* SomePlaceable : PlaceablePool) {
		if (SomePlaceable && SomePlaceable == Placeable) {
			ToRemove = SomePlaceable;
			break;
		}
	}

	if (ToRemove) {
		ToRemove->Destroy();
		PlaceablePool.RemoveSwap(ToRemove);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Placeable in pool was not found."))
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
