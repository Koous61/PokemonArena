#pragma once

#include "CoreMinimal.h"
#include "Pokemon.h"
#include "GameFramework/Character.h"
#include "PokemonArenaCharacter.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;

UCLASS(config=Game)
class APokemonArenaCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY()
	APokemon* CurrentTouchPokemon;

	UPROPERTY()
	APokemon* CurrentPokemon;

public:
	APokemonArenaCharacter();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	void MoveForward(float Val);

	void MoveRight(float Val);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void PickUpOrThrow();

	void Exit();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintCallable)
	APokemon* GetCurrentTouchPokemon() { return CurrentTouchPokemon; }

	UFUNCTION(BlueprintCallable)
	void SetCurrentTouchPokemon(APokemon* NewPokemon);
};
