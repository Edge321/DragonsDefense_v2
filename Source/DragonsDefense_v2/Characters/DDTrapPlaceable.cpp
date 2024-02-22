// Fill out your copyright notice in the Description page of Project Settings.


#include "DDTrapPlaceable.h"
//My classes
#include "../Characters/LivingActor.h"
#include "../Projectile/DDProjectile.h"

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

void ADDTrapPlaceable::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	ALivingActor* LivingActor = Cast<ALivingActor>(OtherActor);

	if (LivingActor) {
		LivingActor->UpdateHealth(Damage);
		Destroy();
	}
	else {
		ADDProjectile* Projectile = Cast<ADDProjectile>(OtherActor);

		if (Projectile) {
			UpdateHealth(Projectile->GetDamage());
		}
	}
}