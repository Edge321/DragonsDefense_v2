// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/BoxComponent.h"
//My classes
#include "../Projectile/DDProjectile.h"
#include "../Characters/DDPlayer.h"
#include "../Game/DDGameModeBase.h"
#include "../Game/DDDifficulty.h"

#define ECC_EnemyChannel ECC_GameTraceChannel1

// Sets default values
AEnemy::AEnemy()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");

	RootComponent = Mesh;
	Collider->SetupAttachment(Mesh);

	//Forces only the collider to have collision
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collider->SetCollisionObjectType(ECC_EnemyChannel);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//Prevents enemy from accelerating like crazy at the beginning
	FloatingPawnMovement->MaxSpeed = MovementSpeed;

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
//Checks distance between the enemy and the castle
void AEnemy::CheckDistance()
{
	//Distance formula on one dimension
	float Distance = FMath::Abs(GetActorLocation().X - Player->GetActorLocation().X);

	if (Distance < DistanceToAttack) {
		FloatingPawnMovement->MaxSpeed = 0;
		StartShooting();
	}	
}

void AEnemy::ApplyModifiers()
{
	EDifficulty Difficulty = EDifficulty::Normal;
	//TODO - Apply modifiers to health, speed, shoot cooldown, and any others
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		Difficulty = GameMode->GetDifficulty();
	}

	switch (Difficulty) {
		case(EDifficulty::Easy):
			Damage *= EasyDamageMod;
			Health *= EasyHealthMod;
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
			MovementSpeed *= HardMovementSpeedMod;
			ShootCooldown *= HardShootCooldownMod;
			SoulValue += HardSoulValueMod;
			break;
		default:
			UE_LOG(LogTemp, Fatal, TEXT("No difficulty exist from GameMode somehow? What the hell is this"))
			break;
	}
}

void AEnemy::OnDeath_Implementation()
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
	if (!IsShooting) { 
		IsShooting = true;
		GetWorldTimerManager().SetTimer(ShootHandle, this, &AEnemy::Shoot, ShootCooldown, true);
	}
}

//TODO - Utilize this somehow. Has not been used yet
void AEnemy::StopShooting()
{
	if (IsShooting) {
		IsShooting = false;
		GetWorldTimerManager().ClearTimer(ShootHandle);
	}
}

void AEnemy::Shoot()
{	
	ADDProjectile* Proj = GetWorld()->SpawnActor<ADDProjectile>(Projectile, GetActorLocation() + ProjectileOffset, GetActorRotation());
	
	if (Proj) {
		Proj->SetProjectileOwner(GetUniqueID());
		FVector Velocity = Proj->GetVelocity();
		Proj->SetVelocity(Velocity * -1);
		Proj->SetCollisionChannelToIgnore(ECC_EnemyChannel);
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