// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "DamageGameplayAbility.h"
#include "ProjectileAbility.generated.h"

class UNiagaraSystem;
class AProjectile;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UProjectileAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& CombatSocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectile> ProjectileClass;
};
