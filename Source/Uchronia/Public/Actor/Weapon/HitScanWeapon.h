// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "RangeWeapon.h"
#include "HitScanWeapon.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class UCHRONIA_API AHitScanWeapon : public ARangeWeapon
{
	GENERATED_BODY()

public:
	virtual void OnConstruction(const FTransform& Transform) override;
	void ApplyWeaponEffects(const FTransform& SocketTransform, const FHitResult& Hit) const;
	virtual void Trigger(const FVector& HitTarget) override;

protected:
	FVector TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget) const;
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutTraceHit) const;
	
	// TODO: Really need to be in DataAsset + AmmoContainer
	// TODO: Might need to change to Niagara 
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	TObjectPtr<UParticleSystem> ImpactParticles;
	
	// TODO: This could be in FireMontage
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	TObjectPtr<UParticleSystem> MuzzleFlash;
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	TObjectPtr<UParticleSystem> BeamParticles;
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	TObjectPtr<UNiagaraSystem> BeamLaser;

	// TODO: This could be in FireMontage
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	TObjectPtr<USoundBase> FireSound;
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	TObjectPtr<USoundBase> ImpactSound;

private:
	/*
	 * Trace End with Scatter
	 * DistanceToSphere, Set accuracy radius at 10m
	 * SphereRadius, Accuracy radius at 10m
	 */
	bool bUseScatter = false;
	float DistanceToSphere = 1000.f;
	float SphereRadius = 75.f;
};
