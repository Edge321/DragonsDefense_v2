// Fill out your copyright notice in the Description page of Project Settings.

#include "DDProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "../Game/DDGameModeBase.h" //TODO - Access difficulty mode to apply to damage modifier
#include "../Characters/LivingActor.h"
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

	ProjectileMovement->Velocity = FVector(800.0f, 0, 0); //Default velocity of projectile 
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
	ALivingActor* Living = Cast<ALivingActor>(OtherActor);

	if (Living) {
		Destroy(); //TODO - Implement bellow
		Living->SetHealth(ProjectileDamage);
	}
}

void ADDProjectile::ApplyModifiers()
{
	// TODO - Adjust damage of projectile according to difficulty chosen
}