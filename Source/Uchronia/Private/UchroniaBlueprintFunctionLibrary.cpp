// Retropsis @ 2023-2024


#include "UchroniaBlueprintFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilityTypes.h"
#include "Game/UchroniaGameMode.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

void UUchroniaBlueprintFunctionLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UUchroniaBlueprintFunctionLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return;
	for(TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetCharacterLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UUchroniaBlueprintFunctionLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AUchroniaGameMode* UchroniaGameMode = Cast<AUchroniaGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(UchroniaGameMode == nullptr) return nullptr;
	return UchroniaGameMode->CharacterClassInfo;
}

bool UUchroniaBlueprintFunctionLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FUchroniaGameplayEffectContext* UchroniaEffectContext = static_cast<const FUchroniaGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return UchroniaEffectContext->IsBlockedHit();
	}
	return false;
}

bool UUchroniaBlueprintFunctionLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FUchroniaGameplayEffectContext* UchroniaEffectContext = static_cast<const FUchroniaGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return UchroniaEffectContext->IsCriticalHit();
	}
	return false;
}

void UUchroniaBlueprintFunctionLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool InIsBlockedHit)
{
	if (FUchroniaGameplayEffectContext* UchroniaEffectContext = static_cast<FUchroniaGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		UchroniaEffectContext->SetIsBlockedHit(InIsBlockedHit);
	}
}

void UUchroniaBlueprintFunctionLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool InIsCriticalHit)
{
	if (FUchroniaGameplayEffectContext* UchroniaEffectContext = static_cast<FUchroniaGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		UchroniaEffectContext->SetIsCriticalHit(InIsCriticalHit);
	}
}

void UUchroniaBlueprintFunctionLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool UUchroniaBlueprintFunctionLibrary::IsHostile(AActor* FirstActor, AActor* SecondActor)
{	
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}
