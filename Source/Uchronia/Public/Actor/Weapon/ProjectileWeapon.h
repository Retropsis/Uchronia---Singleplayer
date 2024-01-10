// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "RangeWeapon.h"
#include "ProjectileWeapon.generated.h"

class UGameplayEffect;
class AAmmoContainer;
class AProjectile;
/**
 *  Weapon spawning a physical projectile, including rocket and grenade launchers 
 */
UCLASS()
class UCHRONIA_API AProjectileWeapon : public ARangeWeapon
{
	GENERATED_BODY()

public:
	virtual void Trigger(const FVector& HitTarget) override;

protected:
	FVector TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget) const;
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutTraceHit);
	
private:
	/*
	 * Trace End with Scatter
	 */
	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties|Scatter")
	bool bUseScatter = false;

	// Set accuracy radius at 10m
	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties|Scatter")
	float DistanceToSphere = 1000.f;

	// Accuracy radius at 10m
	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties|Scatter")
	float SphereRadius = 75.f;
};
