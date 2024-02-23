// Fill out your copyright notice in the Description page of Project Settings.


#include "DDSentientPlaceable.h"
#include "Components/SphereComponent.h"
//My classes
#include "../Characters/Enemy.h"
#include "../Characters/DDPlaceableAI.h"

ADDSentientPlaceable::ADDSentientPlaceable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	AttackCollider = CreateDefaultSubobject<USphereComponent>("AttackCollider");

	RootComponent = Mesh;
	Collider->SetupAttachment(Mesh);
	AttackCollider->SetupAttachment(Mesh);
	Arrow->SetupAttachment(Mesh);

	CurrentAI = PlaceableAI::Deactivated;
}

void ADDSentientPlaceable::BeginPlay()
{
	Super::BeginPlay();

	AttackCollider->SetSphereRadius(AttackRadius);

	AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &ADDSentientPlaceable::OverlapBegin);
	AttackCollider->OnComponentEndOverlap.AddDynamic(this, &ADDSentientPlaceable::OverlapEnd);
}

const UStaticMeshComponent* ADDSentientPlaceable::GetMesh() const
{
	return Mesh;
}

void ADDSentientPlaceable::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	AttackCollider->SetSphereRadius(AttackRadius);
}

void ADDSentientPlaceable::SetAI(const PlaceableAI AIState)
{
	CurrentAI = AIState;
}

void ADDSentientPlaceable::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA<AEnemy>()) {
		return;
	}

	AEnemy* Enemy = Cast<AEnemy>(OtherActor);

	if (Enemy) {
		AddEnemy(Enemy);
	}

	//TODO - Maybe consider like a timer or something each time they attack? 
	// That way placeable can check if AI has changed
	Attack();
}

void ADDSentientPlaceable::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->IsA<AEnemy>()) {
		return;
	}

	AEnemy* Enemy = Cast<AEnemy>(OtherActor);

	if (Enemy) {
		RemoveEnemy(Enemy);
	}

	if (EnemiesInArea.Num() <= 0) {
		Deactive();
	}
}

void ADDSentientPlaceable::Attack() const
{
	switch (CurrentAI) {
		case PlaceableAI::Deactivated:
			//Nothing happens
			break;
		case PlaceableAI::ClosestEnemy:

			break;
		case PlaceableAI::FurthestEnemy:

			break;
		case PlaceableAI::ClosestToCastle:

			break;
		case PlaceableAI::FurthestFromCastle:

			break;
		case PlaceableAI::RoundRobin:

			break;
		case PlaceableAI::CurrentAttacker:

			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("%s does not have a valid AI set"), *GetName())
	}
}

void ADDSentientPlaceable::Deactive()
{
	//Have some things here that reset positioning of placeable
	//or whatever other things that need resetting
	CurrentAI = PlaceableAI::Deactivated;
}

void ADDSentientPlaceable::AddEnemy(AEnemy* Enemy)
{
	if (Enemy) {
		EnemiesInArea.Add(Enemy);
	}
}

void ADDSentientPlaceable::RemoveEnemy(AEnemy* Enemy)
{
	if (Enemy) {
		uint32 ID = Enemy->GetUniqueID();
		for (AEnemy* SomeEnemy : EnemiesInArea) {
			if (SomeEnemy && SomeEnemy->GetUniqueID() == ID) {
				EnemiesInArea.Remove(SomeEnemy);
				break;
			}
		}
	}
}
