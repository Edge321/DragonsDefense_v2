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
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ADDProjectile::OverlapBegin);

	ProjectileMovement->Velocity = FVector(800.0f, 0, 0); //Default velocity of projectile 
}

// Called when the game starts or when spawned
void ADDProjectile::BeginPlay()
{
	Super::BeginPlay();

	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//TODO - Get the current difficulty

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ADDProjectile::EnableCollision, 0.1f, false);
	GetWorldTimerManager().SetTimer(ProjectileTimer, this, &ADDProjectile::DestroySelf, ProjectileLifetime, false);
}

void ADDProjectile::SetVelocity(FVector Velocity)
{
	ProjectileMovement->Velocity = Velocity;
}

void ADDProjectile::SetProjectileOwner(uint32 ActorID)
{
	OwnerID = ActorID;
}

//FVector ADDProjectile::GetVelocity()
//{
//	return ProjectileMovement->Velocity;
//}

void ADDProjectile::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALivingActor* Living = Cast<ALivingActor>(OtherActor);

	if (Living && (Living->GetUniqueID() != OwnerID)) {
		Destroy();
		Living->SetHealth(ProjectileDamage);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString::Printf(TEXT("Health: %f"), Living->GetHealth()));
	}
}

void ADDProjectile::ApplyModifiers()
{
	// TODO - Adjust damage of projectile according to difficulty chosen
}

void ADDProjectile::EnableCollision()
{
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collider->SetCollisionResponseToAllChannels(ECR_Overlap);
	UE_LOG(LogTemp, Log, TEXT("Re-enabled collision!"))
}

void ADDProjectile::DestroySelf()
{
	Destroy();
}