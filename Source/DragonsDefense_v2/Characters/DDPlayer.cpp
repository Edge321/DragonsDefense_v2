// Fill out your copyright notice in the Description page of Project Settings.

#include "DDPlayer.h"
#include "../Projectile/DDProjectile.h"
#include "../Game/DDGameModeBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

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
}

// Called when the game starts or when spawned
void ADDPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	ValidateProjectile();

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

void ADDPlayer::ValidateProjectile()
{
	check(Projectile != nullptr);
}

void ADDPlayer::ResetStats()
{
	//TODO - Reset them stats!
}

void ADDPlayer::LimitArea()
{
	FVector NewLocation = GetActorLocation();
	NewLocation.Y = FMath::Clamp(NewLocation.Y, -AreaLimitY, AreaLimitY);
	SetActorLocation(NewLocation);
}

void ADDPlayer::GameOverEventFunction()
{
	ResetStats();
	APlayerController* PlayController = GetController<APlayerController>();
	DisableInput(PlayController);
	PlayController->bShowMouseCursor = true;
	PlayController->SetInputMode(FInputModeGameAndUI());
}

void ADDPlayer::GameStartEventFunction()
{
	APlayerController* PlayController = GetController<APlayerController>();
	EnableInput(PlayController);
	PlayController->bShowMouseCursor = false;
	PlayController->SetInputMode(FInputModeGameOnly());
}
