#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Pokemon.generated.h"

class UPhysicsHandleComponent;
class APokemonArenaCharacter;

DECLARE_LOG_CATEGORY_EXTERN(LogPokemon, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeHealth, int32, Health);

class UProjectileMovementComponent;
UCLASS()
class POKEMONARENA_API APokemon : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxHandleComponent;

	UFUNCTION()
	void BoxHandleComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void BoxHandleComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxAttackComponent;

	UFUNCTION()
	void BoxAttackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void BoxAttackComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void AttackCooldownFinish();

	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

public:
	APokemon();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	int32 CurrentHealth;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APokemonArenaCharacter* CurrentOwner;
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentHealth () { return CurrentHealth; }

	UFUNCTION(BlueprintCallable)
	int32 GetMaxHealth () { return MaxHealth; }

	UFUNCTION(BlueprintCallable)
	void SetCurrentHealth(int32 NewHealth);

	UFUNCTION(BlueprintCallable)
	void Died();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Avatar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseFire;

	bool bCanUseAttack;

	void Fire();

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnChangeHealth OnChangeHealth;

	UPROPERTY()
	FTimerHandle AttackCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCooldownDuration;
};
