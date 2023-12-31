// Fill out your copyright notice in the Description page of Project Settings.

#include <cmath>

#include "DDPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
//My classes
#include "../Game/DDGameModeBase.h"
#include "../Projectile/DDProjectile.h"

// Sets default values
ADDPlayer::ADDPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatPawnMove");

	RootComponent = Mesh;

	//Forces collider to have the only collision
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collider->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void ADDPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	ValidateProjectile();

	MaxHealth = Health;
	TempHealth = Health;
	TempMovementSpeed = MovementSpeed;
	TempShootSpeed = ShootSpeed;
	TempDamage = Damage;
	Armor = 0;

	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->OnGameOver.AddDynamic(this, &ADDPlayer::GameOverEventFunction);
		GameMode->OnGameStart.AddDynamic(this, &ADDPlayer::GameStartEventFunction);
	}
	//Want to disable the input at the beginning obviously!
	GameOverEventFunction();
}

void ADDPlayer::Tick(float DeltaTime)
{
	LimitArea();
}

// Called to bind functionality to input
void ADDPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

const float ADDPlayer::GetHealth() const
{
	return TempHealth;
}

const float ADDPlayer::GetMaxHealth() const
{
	return MaxHealth;
}

const float ADDPlayer::GetMovementSpeed() const
{
	return TempMovementSpeed;
}

const float ADDPlayer::GetShootSpeed() const
{
	return TempShootSpeed;
}

const float ADDPlayer::GetArmor() const
{
	return Armor;
}

const float ADDPlayer::GetDamage() const
{
	return TempDamage;
}

void ADDPlayer::UpdateHealth(const float HealthModifier)
{
	TempHealth = FMathf::Clamp(TempHealth + HealthModifier, 0, MaxHealth);

	//Temporary until in-game health bars are implemented
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,
		FString::Printf(TEXT("%s's Health: %f"), *GetName(), TempHealth));

	if (TempHealth <= 0)
	{
		OnDeath();
	}
}

void ADDPlayer::UpdateMaxHealth(const float MaxHealthModifier)
{
	MaxHealth = FMathf::Clamp(MaxHealth + MaxHealthModifier, TempHealth, INFINITY);
}

void ADDPlayer::UpdateMovementSpeed(const float MovementSpeedModifier)
{
	TempMovementSpeed = FMathf::Clamp(TempMovementSpeed + MovementSpeedModifier, MovementSpeed, INFINITY);
}

void ADDPlayer::UpdateShootSpeed(const float ShootSpeedModifier)
{
	//Just an arbitrary number. 
	//TODO - Think of the ramifications of this
	float MinShootClamp = 0.1f;
	TempShootSpeed = FMathf::Clamp(TempShootSpeed + ShootSpeedModifier, MinShootClamp, INFINITY);
}

void ADDPlayer::UpdateArmor(const float ArmorModifier)
{
	TempShootSpeed = FMathf::Clamp(Armor + ArmorModifier, 0, INFINITY);
}

void ADDPlayer::UpdateDamage(const float DamageModifier)
{
	//The enemy gotta do some damage you know
	float MaxDamageClamp = -0.1;
	TempDamage = FMathf::Clamp(TempDamage + DamageModifier, -INFINITY, MaxDamageClamp);
}

void ADDPlayer::ValidateProjectile()
{
	check(Projectile != nullptr);
}

void ADDPlayer::ResetStats()
{
	TempHealth = Health;
	MaxHealth = Health;
	TempMovementSpeed = MovementSpeed;
	TempShootSpeed = ShootSpeed;
	Armor = 0;
	TempDamage = Damage;
}

void ADDPlayer::LimitArea()
{
	FVector NewLocation = GetActorLocation();
	NewLocation.Y = FMath::Clamp(NewLocation.Y, -AreaLimitY, AreaLimitY);
	SetActorLocation(NewLocation);
}

void ADDPlayer::OnDeath()
{
	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->GameOver();
	}
}

void ADDPlayer::GameOverEventFunction()
{
	ResetStats();
	APlayerController* PlayController = GetController<APlayerController>();
	DisableInput(PlayController);
	PlayController->bShowMouseCursor = true;
	PlayController->SetInputMode(FInputModeUIOnly());
}

void ADDPlayer::GameStartEventFunction()
{
	APlayerController* PlayController = GetController<APlayerController>();
	EnableInput(PlayController);
	//PlayController->bShowMouseCursor = false;
	PlayController->SetInputMode(FInputModeGameAndUI());
}
