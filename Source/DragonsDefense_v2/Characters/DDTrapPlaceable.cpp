// Fill out your copyright notice in the Description page of Project Settings.


#include "DDTrapPlaceable.h"
//My classes
#include "../Characters/LivingActor.h"
#include "../Characters/Enemy.h"
#include "../Projectile/DDProjectile.h"
#include "../Lib/DDColliderLibrary.h"

// Sets default values
ADDTrapPlaceable::ADDTrapPlaceable()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	RootComponent = Mesh;
	Collider->SetupAttachment(Mesh);
	Arrow->SetupAttachment(Mesh);

	//He was forced to use only collider box
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DDColliderLibrary::SetCollisionChannelToIgnore(Collider, ECC_AttackRadiusChannel);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ADDTrapPlaceable::OverlapBegin);

	Mesh->bRenderCustomDepth = true;
}

// Called when the game starts or when spawned
void ADDTrapPlaceable::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> OverlapActors;
	Collider->GetOverlappingActors(OverlapActors);
	OnSpawnOverlap(OverlapActors);

	if (bIgnoreProjectiles) {
		//Ignore projectiles. It looks like only projectiles are in ECC_WorldDynamic
		DDColliderLibrary::SetCollisionChannelToIgnore(Collider, ECC_WorldDynamic);
	}
}

const UStaticMeshComponent* ADDTrapPlaceable::GetMesh() const
{
	return Mesh;
}

void ADDTrapPlaceable::EnableHighlight()
{
	Mesh->SetCustomDepthStencilValue(Stencil_HighlightDepth);
}

void ADDTrapPlaceable::DisableHighlight()
{
	Mesh->SetCustomDepthStencilValue(0);
}

void ADDTrapPlaceable::OnSpawnOverlap(TArray<AActor*> OverlapActors)
{
	//Just damage the first LivingActor its colliding with
	for (AActor* Actor : OverlapActors) {
		if (Actor && Actor->IsA<ALivingActor>()) {
			ALivingActor* LivingActor = Cast<ALivingActor>(Actor);
			LivingActor->UpdateHealth(Damage);
			OnDeath();
			break;
		}
	}
}

void ADDTrapPlaceable::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (!OtherActor) {
		return;
	}

	ALivingActor* LivingActor = Cast<ALivingActor>(OtherActor);

	if (LivingActor) {
		LivingActor->UpdateHealth(Damage);
		OnDeath(); //TODO - Will be changed later to some general function for future trap classes to use
	}
	else {
		if (OtherActor && OtherActor->IsA<ADDProjectile>()) {
			ADDProjectile* Projectile = Cast<ADDProjectile>(OtherActor);
			const AActor* ProjOwner = Projectile->GetProjectileOwner();
			
			if (ProjOwner->IsA<AEnemy>()) {
				UpdateHealth(Projectile->GetDamage());
			}
		}
	}
}