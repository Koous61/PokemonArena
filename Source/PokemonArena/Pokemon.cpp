#include "Pokemon.h"

#include "PokemonArenaCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

DEFINE_LOG_CATEGORY(LogPokemon);

void APokemon::BoxHandleComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* OverlayCharacter = Cast<APokemonArenaCharacter>(OtherActor);
	if (IsValid(OverlayCharacter))
	{
		if (!OverlayCharacter->GetCurrentTouchPokemon())
		{
			OverlayCharacter->SetCurrentTouchPokemon(this);
			CurrentOwner = OverlayCharacter;
			UE_LOG(LogPokemon, Display, TEXT("Character %s find pokemon %s"), *OverlayCharacter->GetName(), *GetName());
		}
	}
}

void APokemon::AttackCooldownFinish()
{
	bCanUseAttack = true;
}

void APokemon::BoxHandleComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto* OverlayCharacter = Cast<APokemonArenaCharacter>(OtherActor);
	if (IsValid(OverlayCharacter))
	{
		if (OverlayCharacter->GetCurrentTouchPokemon())
		{
			OverlayCharacter->SetCurrentTouchPokemon(nullptr);
			CurrentOwner = nullptr;
			UE_LOG(LogPokemon, Display, TEXT("Character %s not found pokemon %s"), *OverlayCharacter->GetName(), *GetName());
		}
	}
}

void APokemon::BoxAttackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CurrentOwner)
	{
		if (bCanUseAttack)
		{
			auto* OverlayPokemon = Cast<APokemon>(OtherActor);
			if (IsValid(OverlayPokemon))
			{
				if (FMath::RandRange(1, 2) == 1)
				{
					OverlayPokemon->SetCurrentHealth(GetCurrentHealth() - Damage);
					UE_LOG(LogPokemon, Display, TEXT("Pokemon %s attack pokemon %s"), *GetName(), *OverlayPokemon->GetName());
				}
				else
				{
					UE_LOG(LogPokemon, Display, TEXT("Pokemon %s don't lucky to attack pokemon %s"), *GetName(), *OverlayPokemon->GetName());
				}
				bCanUseAttack = false;
				GetWorld()->GetTimerManager().SetTimer(AttackCooldown, this, &APokemon::AttackCooldownFinish, AttackCooldownDuration, false);
			}
		}
	}
}

void APokemon::BoxAttackComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

APokemon::APokemon()
{
	PrimaryActorTick.bCanEverTick = true;

	bCanUseAttack = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	BoxHandleComponent = CreateDefaultSubobject<UBoxComponent>("BoxHandleComponent");
	BoxAttackComponent = CreateDefaultSubobject<UBoxComponent>("BoxAttackComponent");

	BoxHandleComponent->InitBoxExtent(FVector(200,200,200));
	RootComponent = BoxHandleComponent;

	AttackCooldownDuration = 2.0f;
	Damage = 10.0f;

	BoxAttackComponent->InitBoxExtent(FVector(120,120,120));
	BoxAttackComponent->SetupAttachment(GetRootComponent());
	
	StaticMeshComponent->SetupAttachment(GetRootComponent());
}

void APokemon::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	
	BoxHandleComponent->OnComponentBeginOverlap.AddDynamic(this, &APokemon::BoxHandleComponentBeginOverlap);
	BoxHandleComponent->OnComponentEndOverlap.AddDynamic(this, &APokemon::BoxHandleComponentEndOverlap);

	BoxAttackComponent->OnComponentBeginOverlap.AddDynamic(this, &APokemon::BoxAttackComponentBeginOverlap);
	BoxAttackComponent->OnComponentEndOverlap.AddDynamic(this, &APokemon::BoxAttackComponentEndOverlap);

	UE_LOG(LogPokemon, Display, TEXT("Pokemon %s has been spawned"), *GetName());
		
}

void APokemon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APokemon::SetCurrentHealth(int32 NewHealth)
{
	CurrentHealth = NewHealth;
	OnChangeHealth.Broadcast(NewHealth);
	if (CurrentHealth <= 0)
	{
		Died();
	}
}

void APokemon::Died()
{
	UE_LOG(LogPokemon, Display, TEXT("Pokemon %s has been destroyed"), *GetName());
	auto* NewPokemon = GetWorld()->SpawnActor<APokemon>(SpawnLocation.GetLocation(), GetActorRotation());
	NewPokemon->SpawnLocation = SpawnLocation;
	Destroy();
}

void APokemon::Fire()
{
	bUseFire = true;
}

