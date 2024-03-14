// Fill out your copyright notice in the Description page of Project Settings.

#include "DDProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
//My classes
#include "../Game/DDGameModeBase.h"
#include "../Game/DDProjectileManager.h"
#include "../Characters/LivingActor.h"

// Sets default values
ADDProjectile::ADDProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Collider = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileEffects = CreateDefaultSubobject<UNiagaraComponent>("ProjectileEffects");

	RootComponent = Mesh;
	Collider->SetupAttachment(Mesh);
	ProjectileEffects->SetupAttachment(Mesh);

	//This is to make sure only the collider is allowed to collide
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement->Velocity = FVector(800.0f, 0, 0); //Default velocity of projectile 
}

// Called when the game starts or when spawned
void ADDProjectile::BeginPlay()
{
	Super::BeginPlay();

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ADDProjectile::OverlapBegin);

	//Prevents collider from colliding if spawned inside its owner (heehee)
	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		ADDProjectileManager& ProjectileManager = GameMode->GetProjectileManager();
		ProjectileManager.AddProjectileToPool(this);
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ADDProjectile::EnableCollision, 0.1f, false);
	GetWorldTimerManager().SetTimer(ProjectileTimer, this, &ADDProjectile::DestroySelf, ProjectileLifetime, false);
}

void ADDProjectile::SetVelocity(const FVector Velocity)
{
	ProjectileMovement->Velocity = Velocity;
}

const float ADDProjectile::GetDamage() const
{
	return ProjectileDamage;
}

void ADDProjectile::SetDamage(float Damage)
{
	ProjectileDamage = Damage;
}

void ADDProjectile::SetProjectileOwner(const uint32 ActorID)
{
	OwnerID = ActorID;
}

void ADDProjectile::SetProjectileOwner(const AActor* Actor)
{
	uint32 ActorID = Actor->GetUniqueID();
	OwnerID = ActorID;
	ActorOwner = Actor;
}

const AActor* ADDProjectile::GetProjectileOwner() const
{
	return ActorOwner;
}

void ADDProjectile::SetCollisionChannelToIgnore(const ECollisionChannel Channel)
{
	Collider->SetCollisionResponseToChannel(Channel, ECR_Ignore);
}

void ADDProjectile::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALivingActor* Living = Cast<ALivingActor>(OtherActor);

	//Checking if valid pointer and actor isn't hurt from own projectile
	if (Living && (Living->GetUniqueID() != OwnerID)) {
		Living->UpdateHealth(ProjectileDamage);
		DestroySelf();
	}
}

void ADDProjectile::EnableCollision()
{
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ADDProjectile::DestroySelf()
{
	OnProjectileDestroyed.ExecuteIfBound(this);
}