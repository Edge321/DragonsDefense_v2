// Fill out your copyright notice in the Description page of Project Settings.


#include "DDPlaceableManager.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"
//My classes
#include "../Game/DDPlaceablePreview.h"
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

	CheckPreviewValidity();

	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->OnGameOver.AddDynamic(this, &ADDPlaceableManager::GameOverEventFunction);
		GameMode->OnGameStart.AddDynamic(this, &ADDPlaceableManager::GameStartEventFunction);
		GameMode->OnWaveOver.AddDynamic(this, &ADDPlaceableManager::WaveOverEventFunction);
		GameMode->OnWaveStart.AddDynamic(this, &ADDPlaceableManager::WaveStartEventFunction);
	}

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

FVector ADDPlaceableManager::GetPreviewLocation() const
{
	return Preview->GetActorLocation();
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

void ADDPlaceableManager::SpawnPlaceableAtCursor(TSubclassOf<ADDPlaceable> PlaceableClass)
{
	FVector MouseLocation, MouseDirection;

	ADDPlaceable* Placeable = GetWorld()->SpawnActor<ADDPlaceable>(PlaceableClass, GetPreviewLocation(), Preview->GetActorRotation());

	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (Controller) {
		Controller->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("Player controller is null for PlaceableManager, aborting"))
	}

	//TODO - Adjust the position accordingly cus it spawns in the floor
	//FVector PreviewLocation = Preview->GetActorLocation();
	//FVector MeshSize = Preview->GetMeshSize();

	//FVector Adjustment = -MouseDirection * MeshSize.Z;

	//DrawDebugLine(GetWorld(), PreviewLocation, PreviewLocation + Adjustment, FColor::Red, false, 5.0f);

	if (Placeable) {
		//Placeable->AddActorWorldOffset(Adjustment);
		Placeable->SpawnDefaultController();
		Placeable->AutoPossessAI = EAutoPossessAI::Spawned;
		Placeable->OnPlaceableDeath.BindUObject(this, &ADDPlaceableManager::RemovePlaceableFromPool);
		AddPlaceableToPool(Placeable);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Something went wrong with spawning a placeable at the cursor!"))
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
	int32 UniqueID = Placeable->GetUniqueID();

	for (ADDPlaceable* Placeable : PlaceablePool) {
		if (Placeable && Placeable->GetUniqueID() == UniqueID) {
			Placeable->Destroy();
			PlaceablePool.Remove(Placeable);
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
