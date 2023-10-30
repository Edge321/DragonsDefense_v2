// Fill out your copyright notice in the Description page of Project Settings.

#include "DDProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "../Game/DDGameModeBase.h" //TODO - Access difficulty mode to apply to damage modifier
#include "NiagaraComponent.h"

// Sets default values
ADDProjectile::ADDProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Collider = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileEffects = CreateDefaultSubobject<UNiagaraComponent>("ProjectileEffects");

	RootComponent = Mesh;
	Collider->SetupAttachment(Mesh);
	ProjectileEffects->SetupAttachment(Mesh);

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ADDProjectile::OverlapBegin);
}

// Called when the game starts or when spawned
void ADDProjectile::BeginPlay()
{
	Super::BeginPlay();

	//TODO - Get the current difficulty
	
}

void ADDProjectile::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("We hit something boss!"))
	// TODO - Account for damage of whoever was hit, either the player or the enemy by casting to their respective class
	// Do like a switch statement
}

void ADDProjectile::ApplyModifiers()
{
	// TODO - Adjust damage of projectile according to difficulty chosen
}