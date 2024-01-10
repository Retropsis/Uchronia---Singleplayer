// Retropsis @ 2023-2024


#include "Actor/Weapon/Round.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ARound::ARound()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->SetIsReplicated(true);
	ProjectileMovementComponent->bShouldBounce = true;
}

void ARound::BeginPlay()
{
	AActor::BeginPlay();
	SpawnTrailSystem();
	DestroyTimerStart();
	
	ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &ARound::OnBounce);
}

void ARound::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if(IsValid(BounceSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, BounceSound, GetActorLocation());
	}
}

void ARound::Destroyed()
{
	ApplyRadialDamageWithFalloff();
	Super::Destroyed();
}