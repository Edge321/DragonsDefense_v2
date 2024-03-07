// Fill out your copyright notice in the Description page of Project Settings.


#include "DDTrapPlaceable.h"
//My classes
#include "../Characters/LivingActor.h"
#include "../Projectile/DDProjectile.h"

#define ECC_AttackRadiusChannel ECC_GameTraceChannel4

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
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ADDTrapPlaceable::OverlapBegin);
	Collider->SetCollisionResponseToChannel(ECC_AttackRadiusChannel, ECR_Ignore);

	Mesh->bRenderCustomDepth = true;
}

// Called when the game starts or when spawned
void ADDTrapPlaceable::BeginPlay()
{
	Super::BeginPlay();
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

void ADDTrapPlaceable::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	ALivingActor* LivingActor = Cast<ALivingActor>(OtherActor);

	if (LivingActor) {
		LivingActor->UpdateHealth(Damage);
		Destroy();
	}
	else {
		//BUG - Projectile for placeables still damage the trap. Should only be enemy's projectiles
		//Maybe add tags to the projectile?
		ADDProjectile* Projectile = Cast<ADDProjectile>(OtherActor);

		if (Projectile) {
			UpdateHealth(Projectile->GetDamage());
		}
	}
}