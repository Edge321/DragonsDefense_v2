// Fill out your copyright notice in the Description page of Project Settings.

#include "DDPlayer.h"
#include <cmath>
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"
//My classes
#include "../Game/DDGameModeBase.h"
#include "../Projectile/DDProjectile.h"
#include "../Characters/DDPlaceable.h"

#define ECC_PlaceableChannel ECC_GameTraceChannel2

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
	Arrow->SetupAttachment(Mesh);
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

	OriginalLocation = GetActorLocation();

	ADDGameModeBase* GameMode = Cast<ADDGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->OnGameOver.AddDynamic(this, &ADDPlayer::GameOverEventFunction);
		GameMode->OnGameStart.AddDynamic(this, &ADDPlayer::GameStartEventFunction);
		GameMode->OnWaveOver.AddDynamic(this, &ADDPlayer::WaveOverEventFunction);
		GameMode->OnWaveStart.AddDynamic(this, &ADDPlayer::WaveStartEventFunction);
		GameMode->OnPlacing.AddDynamic(this, &ADDPlayer::PlacementEventFunction);
	}
	//Want to disable the input at the beginning obviously!
	GameOverEventFunction();
}

void ADDPlayer::FindPlaceableWithCursor()
{
	FVector MouseLocation, MouseDirection;

	FVector2D MouseScreenPosition;
	FHitResult Hit;

	APlayerController* PController = UGameplayStatics::GetPlayerController(this, 0);

	if (PController) {
		PController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);
		PController->GetMousePosition(MouseScreenPosition.X, MouseScreenPosition.Y);
	}
	else {
		UE_LOG(LogTemp, Fatal, TEXT("Player controller is null for Player, aborting"))
	}
	
	FVector End = (MouseDirection * 10000) + MouseLocation;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PlaceableChannel));

	TArray<AActor*> ActorsToIgnore;
	
	//Lots of things happening here, so
	//Basically just returns a FHitResult on the object the line trace collides with first
	bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		MouseLocation,
		End,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		Hit,
		true);

	if (bHit) {
		ADDPlaceable* Placeable = Cast<ADDPlaceable>(Hit.GetActor());
		if (Placeable) {
			OnClickPlaceable.Broadcast(MouseScreenPosition, Placeable);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Error: Actor clicked on is NOT a placeable"))
		}
	}
	else {
		OnClickOutsideOfPlaceable.Broadcast();
	}
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

const float ADDPlayer::GetDamage() const
{
	return TempDamage;
}

const bool ADDPlayer::IsPlacingState() const
{
	return bIsPlacing;
}

void ADDPlayer::UpdateHealth(const float HealthModifier)
{
	TempHealth = FMathf::Clamp(TempHealth + HealthModifier, 0, MaxHealth);

	if (TempHealth <= 0)
	{
		OnDeath();
	}

	OnUpdateHealth.Broadcast(TempHealth, MaxHealth);
}

void ADDPlayer::UpdateMaxHealth(const float MaxHealthModifier)
{
	MaxHealth = FMathf::Clamp(MaxHealth + MaxHealthModifier, TempHealth, INFINITY);
	OnUpdateHealth.Broadcast(TempHealth, MaxHealth);
}

void ADDPlayer::UpdateMovementSpeed(const float MovementSpeedModifier)
{
	TempMovementSpeed = FMathf::Clamp(TempMovementSpeed + MovementSpeedModifier, MovementSpeed, INFINITY);
}

void ADDPlayer::UpdateShootSpeed(const float ShootSpeedModifier)
{
	//Just an arbitrary number. Prevents shoot speed going down to 0 (basically shooting every frame)
	float MinShootClamp = 0.1f;
	TempShootSpeed = FMathf::Clamp(TempShootSpeed + ShootSpeedModifier, MinShootClamp, INFINITY);
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
	PlayController->SetInputMode(FInputModeGameAndUI());
	SetActorLocation(OriginalLocation);
}

void ADDPlayer::WaveOverEventFunction()
{
	APlayerController* PlayController = GetController<APlayerController>();
	DisableInput(PlayController);
	PlayController->bShowMouseCursor = true;
	PlayController->SetInputMode(FInputModeUIOnly());
}

void ADDPlayer::WaveStartEventFunction()
{
	GameStartEventFunction();
}

void ADDPlayer::PlacementEventFunction(bool IsPlacing)
{
	bIsPlacing = IsPlacing;
}
