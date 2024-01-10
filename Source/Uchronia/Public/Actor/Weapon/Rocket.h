// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Actor/Weapon/Projectile.h"
#include "Rocket.generated.h"

class URocketMovementComponent;
/**
 * 
 */
UCLASS()
class UCHRONIA_API ARocket : public AProjectile
{
	GENERATED_BODY()

public:
	ARocket();
	virtual void Destroyed() override;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URocketMovementComponent> RocketMovementComponent;

	UPROPERTY(EditDefaultsOnly, Category="Projectile Properties")
	TObjectPtr<USoundBase> ProjectileLoopSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> ProjectileLoopComponent;

	UPROPERTY(EditDefaultsOnly, Category="Projectile Properties")
	TObjectPtr<USoundAttenuation> ProjectileLoopAttenuation;
};
