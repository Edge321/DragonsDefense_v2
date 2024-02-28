// Fill out your copyright notice in the Description page of Project Settings.


#include "DDSentientPlaceable.h"
#include "Components/SphereComponent.h"
//My classes
#include "../Characters/Enemy.h"
#include "../Characters/DDPlaceableAI.h"
#include "../Projectile/DDProjectile.h"

#define ECC_PreviewChannel ECC_GameTraceChannel3

ADDSentientPlaceable::ADDSentientPlaceable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	AttackCollider = CreateDefaultSubobject<USphereComponent>("AttackCollider");
	RadiusMesh = CreateDefaultSubobject<UStaticMeshComponent>("AttackRadius");

	RootComponent = Mesh;
	Collider->SetupAttachment(Mesh);
	AttackCollider->SetupAttachment(Mesh);
	Arrow->SetupAttachment(Mesh);
	RadiusMesh->SetupAttachment(Mesh);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RadiusMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCollider->SetCollisionObjectType(ECC_PlaceableChannel);
	AttackCollider->SetCollisionResponseToChannel(ECC_PreviewChannel, ECollisionResponse::ECR_Ignore);

	AttackCollider->OnComponentBeginOverlap.AddDynamic(this, &ADDSentientPlaceable::OverlapBegin);
	AttackCollider->OnComponentEndOverlap.AddDynamic(this, &ADDSentientPlaceable::OverlapEnd);

	CurrentAI = PlaceableAI::ClosestToCastle;
}

void ADDSentientPlaceable::BeginPlay()
{
	Super::BeginPlay();

	AttackCollider->SetSphereRadius(AttackRadius);

	float RadiusMeshDiameter = GetRadiusMeshSize().Y;
	float CorrectedScale = AttackRadius / (RadiusMeshDiameter / 2);
	FVector Scale(1, CorrectedScale, CorrectedScale);

	RadiusMesh->SetRelativeScale3D(Scale);
}

const UStaticMeshComponent* ADDSentientPlaceable::GetMesh() const
{
	return Mesh;
}

const float ADDSentientPlaceable::GetAttackRadius() const
{
	return AttackRadius;
}

const FVector ADDSentientPlaceable::GetRadiusMeshSize() const
{
	return RadiusMesh->GetStaticMesh()->GetBounds().GetBox().GetSize();
}

void ADDSentientPlaceable::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	AttackCollider->SetSphereRadius(AttackRadius);

	float RadiusMeshDiameter = GetRadiusMeshSize().Y;
	float CorrectedScale = AttackRadius / (RadiusMeshDiameter / 2);
	FVector Scale(1, CorrectedScale, CorrectedScale);

	RadiusMesh->SetRelativeScale3D(Scale);
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

void ADDSentientPlaceable::Attack()
{
	if (EnemiesInArea.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Enemies not in area but %s is still attacking"), *GetName())
		return;
	}

	switch (CurrentAI) {
		case PlaceableAI::Deactivated:
			UE_LOG(LogTemp, Warning, TEXT("%s is deactivated but in attack state"), *GetName())
			break;
		case PlaceableAI::ClosestEnemy:
			AttackEnemy(FindMinMaxEnemy(false, [&](AEnemy* Enemy) -> float {
				return FVector::Distance(Enemy->GetActorLocation(), GetActorLocation());
				}));
			break;
		case PlaceableAI::FurthestEnemy:
			AttackEnemy(FindMinMaxEnemy(true, [&](AEnemy* Enemy) -> float {
				return FVector::Distance(Enemy->GetActorLocation(), GetActorLocation());
				}));
			break;
		case PlaceableAI::ClosestToCastle:
			AttackEnemy(FindMinMaxEnemy(false, [](AEnemy* Enemy) -> float {
				return Enemy->GetDistanceFromCastle();
				}));
			break;
		case PlaceableAI::FurthestFromCastle:
			AttackEnemy(FindMinMaxEnemy(true, [](AEnemy* Enemy) -> float {
				return Enemy->GetDistanceFromCastle();
				}));
			break;
		case PlaceableAI::RoundRobin:
			AttackEnemy(EnemiesInArea[RobinIndex]);
			RobinIndex = (RobinIndex + 1) % EnemiesInArea.Num();
			break;
		case PlaceableAI::CurrentAttacker:
			//TODO - Gather list of Enemies attacking placeable
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
		Proj->SetProjectileOwner(this);
		Proj->SetCollisionChannelToIgnore(ECC_PlaceableChannel);
		
		FVector EnemyLocation = Enemy->GetActorLocation();
		FVector PlaceLocation = GetActorLocation();
		FVector ProjDirection = EnemyLocation - PlaceLocation;
		ProjDirection.Normalize();

		Proj->SetVelocity(ProjDirection * ProjectileSpeed);
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

AEnemy* ADDSentientPlaceable::FindMinMaxEnemy(bool IsMax, std::function<float(AEnemy*)> ValueFunc) const
{
	if (EnemiesInArea.Num() <= 0) {
		UE_LOG(LogTemp, Error, TEXT("Error: EnemiesInArea array is empty"))
		return nullptr;
	}

	AEnemy* ChosenEnemy = EnemiesInArea[0];

	if (!ChosenEnemy) {
		UE_LOG(LogTemp, Error, TEXT("Error: First Enemy is nullptr"))
		return nullptr;
	}

	float CurrentValue = ValueFunc(ChosenEnemy);

	for (AEnemy* SomeEnemy : EnemiesInArea) {
		if (SomeEnemy) {
			float TmpDist = ValueFunc(SomeEnemy);
			if (IsMax ? (TmpDist > CurrentValue) : (TmpDist < CurrentValue)) {
				ChosenEnemy = SomeEnemy;
				CurrentValue = TmpDist;
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Invalid pointer found in EnemiesInArea area"))
		}
	}

	return ChosenEnemy;
}
