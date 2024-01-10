// Retropsis @ 2023-2024


#include "Actor/Weapon/Bullet.h"

#include "GameFramework/ProjectileMovementComponent.h"

ABullet::ABullet()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->SetIsReplicated(true);
	// ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	// ProjectileMovementComponent->MaxSpeed = InitialSpeed;
}
