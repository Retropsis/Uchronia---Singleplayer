// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UchroniaBlueprintFunctionLibrary.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UUchroniaBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="UchroniaBlueprintFunctionLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="UchroniaBlueprintFunctionLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category="UchroniaBlueprintFunctionLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="UchroniaBlueprintFunctionLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="UchroniaBlueprintFunctionLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="UchroniaBlueprintFunctionLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool InIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category="UchroniaBlueprintFunctionLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool InIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category="UchroniaBlueprintFunctionLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category="UchroniaBlueprintFunctionLibrary|GameplayMechanics")
	static bool IsHostile(AActor* FirstActor, AActor* SecondActor);
};
