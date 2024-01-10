// Retropsis @ 2023-2024


#include "AbilitySystem/UchroniaAbilitySystemGlobals.h"

#include "AbilityTypes.h"

FGameplayEffectContext* UUchroniaAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FUchroniaGameplayEffectContext();
}
