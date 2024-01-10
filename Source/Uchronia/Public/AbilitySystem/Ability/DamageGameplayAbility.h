// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "DamageGameplayAbility.generated.h"

struct FTaggedMontage;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UDamageGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
protected:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
};
