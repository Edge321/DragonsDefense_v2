// Fill out your copyright notice in the Description page of Project Settings.


#include "DDSentientPlaceable.h"
#include "Components/SphereComponent.h"
//My classes
#include "../Characters/Enemy.h"
#include "../Characters/DDPlaceableAI.h"
#include "../Projectile/DDProjectile.h"

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

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCollider->SetCollisionObjectType(ECC_PlaceableChannel);

	AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &ADDSentientPlaceable::OverlapBegin);
	AttackCollider->OnComponentEndOverlap.AddDynamic(this, &ADDSentientPlaceable::OverlapEnd);

	CurrentAI = PlaceableAI::ClosestEnemy;
}

void ADDSentientPlaceable::BeginPlay()
{
	Super::BeginPlay();

	AttackCollider->SetSphereRadius(AttackRadius);
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

	StartAttack();
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

void ADDSentientPlaceable::ValidateProjectile()
{
	check(Projectile != nullptr)
}

void ADDSentientPlaceable::Attack() const
{
	if (EnemiesInArea.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Enemies not in area but still %s is still attacking?"), *GetName())
		return;
	}

	switch (CurrentAI) {
		case PlaceableAI::Deactivated:
			UE_LOG(LogTemp, Warning, TEXT("%s is deactivated but in attack state"), *GetName())
			break;
		case PlaceableAI::ClosestEnemy:
			AttackEnemy(EnemiesInArea[0]);
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

void ADDSentientPlaceable::StartAttack()
{
	if (!bIsAttacking) {
		bIsAttacking = true;
		GetWorldTimerManager().SetTimer(AttackHandle, this, &ADDSentientPlaceable::Attack, AttackSpeed, true);
	}
}

void ADDSentientPlaceable::StopAttack()
{
	if (bIsAttacking) {
		bIsAttacking = false;
		GetWorldTimerManager().ClearTimer(AttackHandle);
	}
}

void ADDSentientPlaceable::AttackEnemy(AEnemy* Enemy) const
{
	//TODO - Account for enemy speed so the placeable can shoot the projectile without chance of missing enemy
	ADDProjectile* Proj = GetWorld()->SpawnActor<ADDProjectile>(Projectile, GetActorLocation(), GetActorRotation());
	if (Proj) {
		FVector EnemyLocation = Enemy->GetActorLocation();
		FVector PlaceLocation = GetActorLocation();
		FVector ProjDirection = EnemyLocation - PlaceLocation;
		ProjDirection.Normalize();

		Proj->SetVelocity(ProjDirection * ProjectileSpeed);
		Proj->SetProjectileOwner(this);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s failed to spawn projectile!"), *GetName())
	}
}

void ADDSentientPlaceable::Deactive()
{
	//Have some things here that reset positioning of placeable
	//or whatever other things that need resetting
	//CurrentAI = PlaceableAI::Deactivated;
	StopAttack();
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
