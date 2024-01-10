// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "UchroniaAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class UCHRONIA_API UUchroniaAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
