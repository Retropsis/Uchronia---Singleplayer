// Retropsis @ 2023-2024

#include "Actor/Weapon/MultiHitScanWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"

void AMultiHitScanWeapon::Trigger(const FVector& HitTarget)
{
	ARangeWeapon::Trigger(HitTarget);
	
	if (const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash")))
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();

		// Registering Hits and Applying Effects
		TMap<UAbilitySystemComponent*, uint32> HitMap;
		for(uint32 i = 0; i < NumHits; i++)
		{
			FHitResult ScanHit;
			WeaponTraceHit(Start, HitTarget, ScanHit);
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ScanHit.GetActor());
			if (TargetASC && HasAuthority())
			{
				if(HitMap.Contains(TargetASC))
				{
					HitMap[TargetASC]++; 
				}
				else
				{
					HitMap.Emplace(TargetASC, 1);
				}
				UWorld* World = GetWorld();
				if (ImpactParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticles, ScanHit.ImpactPoint, ScanHit.ImpactNormal.Rotation());
				}
				if(ImpactSound)
				{
					UGameplayStatics::PlaySoundAtLocation(World, ImpactSound, ScanHit.ImpactPoint, .5f, FMath::FRandRange(-.5f, .5f));
				}
			}
		}

		// Applying Damage
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
		for (TTuple<UAbilitySystemComponent*, unsigned> HitPair : HitMap)
		{
			if(SourceASC && HasAuthority())
			{
				const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, SourceASC->MakeEffectContext());
				
				const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
				
				for (TTuple<FGameplayTag, FScalableFloat>& Pair : DamageTypes)
				{
					const float ScaledDamage = Pair.Value.GetValueAtLevel(1.f);
					UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage * HitPair.Value);
				}
				if (HitPair.Key) // TODO: Probably don't need this check cause its checked earlier already
				{
					HitPair.Key->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}
	}
}
