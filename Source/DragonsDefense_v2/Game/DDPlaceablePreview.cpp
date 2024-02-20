// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "DDPlaceablePreview.h"
//My classes
#include "../Game/DDGameModeBase.h"
#include "../Characters/DDPlayer.h"

// Sets default values
ADDPlaceablePreview::ADDPlaceablePreview()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ADDPlaceablePreview::BeginPlay()
{
	Super::BeginPlay();
	
	GetControllerReference();
	//TODO - Worry about later if collsion and phsyics and all that will be a problem
}

// Called every frame
void ADDPlaceablePreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsHidden()) {
		DisplayPreview();
	}
}

const FVector ADDPlaceablePreview::GetMeshSize() const
{
	return Mesh->GetStaticMesh()->GetBounds().GetBox().GetSize();
}

void ADDPlaceablePreview::SetMesh(UStaticMesh* NewMesh)
{
	if (NewMesh) {
		Mesh->SetStaticMesh(NewMesh);
	}
}

void ADDPlaceablePreview::SetScale(FVector Scale)
{
	Mesh->SetWorldScale3D(Scale);
}

void ADDPlaceablePreview::GetControllerReference()
{
	Controller = UGameplayStatics::GetPlayerController(this, 0);
}

void ADDPlaceablePreview::DisplayPreview()
{
	FVector MouseLocation, MouseDirection;
	FHitResult Hit;
	
	if (Controller) {
		Controller->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("Player controller is null for PlaceablePreview, aborting"))
	}

	//TODO - Account for the scale of the mesh since the BoundingBox uses the original size of the mesh
	
	//Math for adjusting the mesh to be on top of the floor instead of in it
	FVector Size = GetMeshSize();
	float DeltaZ = Size.Z / 2;
	float DeltaX = DeltaZ * (MouseDirection.X / MouseDirection.Z);

	FVector AdjustedOffset(DeltaX, 0, DeltaZ);

	//The big multiplication is to make sure to go far out to almost guarantee a hit from LineTrace
	FVector End = (MouseDirection * 10000) + MouseLocation;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	
	TArray<AActor*> ActorsToIgnore;

	//Lots of things happening here, so
	//Basically just returns a FHitResult on the object the line trace collides with first
	bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		this, 
		MouseLocation, 
		End, 
		ObjectTypes, 
		false, 
		ActorsToIgnore, 
		EDrawDebugTrace::None, 
		Hit, 
		true);

	if (bHit) {
		SetActorLocation(Hit.Location + AdjustedOffset);
	}
}

