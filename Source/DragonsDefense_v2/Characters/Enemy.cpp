// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/MaterialBillboardComponent.h"
//My classes
#include "../Projectile/DDProjectile.h"
#include "../Characters/DDPlayer.h"
#include "../Characters/DDPlaceable.h"
#include "../Game/DDGameModeBase.h"
#include "../Game/DDDifficulty.h"
#include "../UI/DDEnemyHealthBar.h"
#include "../Lib/DDColliderLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");
	AttackSight = CreateDefaultSubobject<UBoxComponent>("AttackSight");

	RootComponent = Mesh;
	Collider->SetupAttachment(Mesh);
	Arrow->SetupAttachment(Mesh);
	AttackSight->SetupAttachment(Mesh);

	//Forces only the collider to have collision. 
	//Whether this is a good idea, I dont know
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collider->SetCollisionObjectType(ECC_EnemyChannel);
	AttackSight->SetCollisionObjectType(ECC_AttackRadiusChannel);
	DDColliderLibrary::SetCollisionChannelToIgnore(AttackSight, ECC_AttackRadiusChannel);

	AttackSight->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OverlapBegin);
	AttackSight->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OverlapEnd);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//Prevents enemy from accelerating like crazy at the beginning
	FloatingPawnMovement->MaxSpeed = MovementSpeed;
	TempHealth = Health;

	ValidateProjectile();
	FindPlayer();
	ApplyModifiers();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector ActualMovement = FVector(DeltaTime * MovementSpeed * -1.0, 0, 0);
	FloatingPawnMovement->AddInputVector(ActualMovement);

	CheckDistance();
}

void AEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	AdjustAttackBox();
}

UStaticMeshComponent* AEnemy::GetMeshComponent() const
{
	return Mesh;
}
UFloatingPawnMovement* AEnemy::GetFloatingPawnMovement() const
{
	return FloatingPawnMovement;
}

const float AEnemy::GetMovementSpeed() const
{
	return MovementSpeed;
}

const float AEnemy::GetDistanceFromCastle() const 
{
	return DistanceFromCastle;
}

const float AEnemy::GetMaxSpeed() const
{
	return FloatingPawnMovement->GetMaxSpeed();
}

//Checks distance between the enemy and the castle
void AEnemy::CheckDistance()
{
	//Distance formula on one dimension
	DistanceFromCastle = FMath::Abs(GetActorLocation().X - Player->GetActorLocation().X);

	if (DistanceFromCastle < DistanceToAttack) {
		StopMoving();
		StartShooting();
	}	
}

void AEnemy::ApplyModifiers()
{
	EDifficulty Difficulty = EDifficulty::Normal;

	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		Difficulty = GameMode->GetDifficulty();
	}

	switch (Difficulty) {
		case(EDifficulty::Easy):
			Damage *= EasyDamageMod;
			Health *= EasyHealthMod;
			TempHealth = Health;
			MovementSpeed *= EasyMovementSpeedMod;
			ShootCooldown *= EasyShootCooldownMod;
			SoulValue += EasySoulValueMod;
			break;
		case(EDifficulty::Normal):
			//Nothing changes
			break;
		case(EDifficulty::Hard):
			Damage *= HardDamageMod;
			Health *= HardHealthMod;
			TempHealth = Health;
			MovementSpeed *= HardMovementSpeedMod;
			ShootCooldown *= HardShootCooldownMod;
			SoulValue += HardSoulValueMod;
			break;
		default:
			UE_LOG(LogTemp, Fatal, TEXT("No difficulty exist from GameMode somehow? What the hell is this"))
			break;
	}
}

void AEnemy::UpdateHealth(const float HealthModifier)
{
	TempHealth += HealthModifier;

	if (TempHealth <= 0) {
		OnDeath();
	}

	UpdateHealthBar();
}

void AEnemy::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bEnableAttackPlaceables) {
		if (OtherActor && OtherActor->IsA<ADDPlaceable>()) {
			ADDPlaceable* Place = Cast<ADDPlaceable>(OtherActor);

			PlaceablesInSight.Add(Place);

			StartShootingPlaceable();
			StopMoving();
		}
	}
}

void AEnemy::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bEnableAttackPlaceables) {
		if (OtherActor && OtherActor->IsA<ADDPlaceable>()) {
			ADDPlaceable* Place = Cast<ADDPlaceable>(OtherActor);

			PlaceablesInSight.Remove(Place);

			if (PlaceablesInSight.Num() <= 0) {
				StopShooting();
				StartMoving();
			}
		}
	}
}

void AEnemy::OnDeath()
{
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->AddScore(Score);
		GameMode->UpdateSouls(SoulValue);
	}

	OnEnemyDeath.ExecuteIfBound(this);
}

void AEnemy::StartShooting()
{	
	//Boolean prevents the Timer Handle from resetting every tick
	if (!bIsShooting) { 
		bIsShooting = true;
		GetWorldTimerManager().SetTimer(ShootHandle, this, &AEnemy::Shoot, ShootCooldown, true);
	}
}

void AEnemy::StopShooting()
{
	if (bIsShooting) {
		bIsShooting = false;
		GetWorldTimerManager().ClearTimer(ShootHandle);
	}
}

void AEnemy::StartMoving()
{
	FloatingPawnMovement->MaxSpeed = MovementSpeed;
}

void AEnemy::StopMoving()
{
	FloatingPawnMovement->MaxSpeed = 0;
}

void AEnemy::Shoot() const
{	
	ADDProjectile* Proj = GetWorld()->SpawnActor<ADDProjectile>(Projectile, GetActorLocation() + ProjectileOffset, GetActorRotation());
	
	if (Proj) {
		Proj->SetProjectileOwner(GetUniqueID());
		//Shoot negative x axis just cus castle is that direction
		//Honestly could use vectors to do this dynamically, maybe later
		FVector Velocity(-ProjectileSpeed, 0, 0);
		Proj->SetVelocity(Velocity);
		Proj->SetCollisionChannelToIgnore(ECC_EnemyChannel);
		Proj->SetCollisionChannelToIgnore(ECC_AttackRadiusChannel);
		Proj->SetDamage(Damage);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ERROR: Enemy projectile unable to spawn"))
	}
}

void AEnemy::StartShootingPlaceable()
{
	if (!bIsShooting) {
		bIsShooting = true;
		GetWorldTimerManager().SetTimer(ShootHandle, this, &AEnemy::ShootPlaceable, ShootCooldown, true);
	}
}

void AEnemy::ShootPlaceable() const
{
	if (PlaceablesInSight.Num() <= 0) {
		UE_LOG(LogTemp, Error, TEXT("There are no placeables in sight!"))
		return;
	}

	ADDProjectile* Proj = GetWorld()->SpawnActor<ADDProjectile>(Projectile, GetActorLocation() + ProjectileOffset, GetActorRotation());

	if (Proj) {
		Proj->SetProjectileOwner(GetUniqueID());

		//Always shooting first Placeable
		ADDPlaceable* Placeable = PlaceablesInSight[0];

		if (Placeable) {
			FVector PlaceLoc = Placeable->GetActorLocation();
			FVector ThisLoc = GetActorLocation();
			FVector Direction = PlaceLoc - ThisLoc;
			Direction.Normalize();

			Proj->SetVelocity(Direction * ProjectileSpeed);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Placeable fetched is not valid"))
				//Is there a reason for doing this? Projectile already has a default velocity
			FVector Velocity(-ProjectileSpeed, 0, 0);
			Proj->SetVelocity(Velocity);
		}
		
		Proj->SetCollisionChannelToIgnore(ECC_EnemyChannel);
		Proj->SetCollisionChannelToIgnore(ECC_AttackRadiusChannel);
		Proj->SetDamage(Damage);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ERROR: Enemy projectile unable to spawn"))
	}
}

void AEnemy::ValidateProjectile()
{
	check(Projectile != nullptr);
}

void AEnemy::FindPlayer()
{
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		// You know, me having this line of code defeats the entire 
		// purpose of GetPlayer returning only a reference. Very awesome
		Player = &(GameMode->GetPlayer());
	}
}

void AEnemy::AdjustAttackBox()
{
	FVector Origin, BoxExtent;
	Mesh->GetLocalBounds(Origin, BoxExtent);
	
	FVector NewBox(AttackSightDistance, BoxExtent.Y, BoxExtent.Z);
	AttackSight->SetBoxExtent(NewBox);

	FVector NewLocation(AttackSightDistance, 0, 0);
	AttackSight->SetRelativeLocation(-NewLocation);
}