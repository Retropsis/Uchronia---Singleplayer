// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;
class UBoxComponent;

UCLASS()
class UCHRONIA_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	// virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	virtual void DestroyTimerEnd();
	virtual void DestroyTimerStart();
	virtual void SpawnTrailSystem();
	virtual void ApplyRadialDamageWithFalloff();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> CollisionBox;

	/*
	 * Mesh & VFX
	 * TODO: Should be DataAsset
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile Properties")
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	// TODO: Might need to change to Niagara 
	UPROPERTY(EditAnywhere, Category="Projectile Properties|VFX")
	TObjectPtr<UParticleSystem> ImpactParticles;

	UPROPERTY(EditAnywhere, Category="Projectile Properties|SFX")
	TObjectPtr<USoundBase> ImpactSound;
	
	// TODO: Might need to change to Niagara 
	UPROPERTY(EditAnywhere, Category="Projectile Properties|VFX")
	TObjectPtr<UParticleSystem> Tracer;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile Properties|VFX")
	TObjectPtr<UNiagaraSystem> TrailSystem;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> TrailSystemComponent;

	UPROPERTY(EditDefaultsOnly, Category="Projectile Properties|VFX")
	FName TrailSocketName = FName("TrailSystemSocket");
	/*
	 *  Mesh & VFX End
	*/

private:	
	TObjectPtr<UParticleSystemComponent> TracerComponent;

	UPROPERTY(EditAnywhere, Category="Projectile Properties|Radial Damage")
	float DamageRadius = 500.f;
	
	UPROPERTY(EditAnywhere, Category="Projectile Properties|Radial Damage")
	float DamageInnerRadius = 200.f;
	
	UPROPERTY(EditAnywhere, Category="Projectile Properties|Radial Damage")
	float DamageOuterRadius = 500.f;
	
	UPROPERTY(EditAnywhere, Category="Projectile Properties|Radial Damage")
	float DamageFalloff = 1.f;	
	
	UPROPERTY(EditAnywhere, Category="Projectile Properties|Radial Damage")
	float MinimumDamage = 10.f;	
	
	/*
	 * Destroy Timer
	*/
	FTimerHandle DestroyTimer;

	UPROPERTY(EditDefaultsOnly, Category="Projectile Properties|Destroy")
	float DestroyTime = 3.f;
	/*
	 * Destroy Timer End
	*/

public:	

};
