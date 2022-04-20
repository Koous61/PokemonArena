#include "PokemonArenaCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"

APokemonArenaCharacter::APokemonArenaCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	BaseLookUpRate = 45.f;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
}

void APokemonArenaCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APokemonArenaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("PickUpOrThrow", IE_Pressed, this, &APokemonArenaCharacter::PickUpOrThrow);
	PlayerInputComponent->BindAction("Exit", IE_Pressed, this, &APokemonArenaCharacter::Exit);
	PlayerInputComponent->BindAxis("MoveForward", this, &APokemonArenaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APokemonArenaCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APokemonArenaCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APokemonArenaCharacter::LookUpAtRate);
}

void APokemonArenaCharacter::SetCurrentTouchPokemon(APokemon* NewPokemon)
{
	CurrentTouchPokemon = NewPokemon;
}

void APokemonArenaCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APokemonArenaCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APokemonArenaCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * 2 * GetWorld()->GetDeltaSeconds());
}

void APokemonArenaCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APokemonArenaCharacter::PickUpOrThrow()
{
	if (IsValid(CurrentPokemon))
	{
		CurrentPokemon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentPokemon->Fire();
		CurrentPokemon = nullptr;
	}
	else
	{
		if (IsValid(CurrentTouchPokemon))
		{
			CurrentPokemon = CurrentTouchPokemon;
			CurrentTouchPokemon = nullptr;
			CurrentPokemon->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}

void APokemonArenaCharacter::Exit()
{
	FGenericPlatformMisc::RequestExit(true);
}

