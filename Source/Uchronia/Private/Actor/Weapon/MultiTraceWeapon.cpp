// Retropsis @ 2023-2024

#include "Actor/Weapon/MultiTraceWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "Actor/Weapon/Projectile.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Interaction/CombatInterface.h"

void AMultiTraceWeapon::Trigger(const FVector& HitTarget)
{
	ARangeWeapon::Trigger(HitTarget);
	
	if(!HasAuthority()) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	
	if (const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash")))
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();

		// Can prepare Damage Spec in advance for each pellet, fragment, projectile
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, SourceASC->MakeEffectContext());
		const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();

		for (TTuple<FGameplayTag, FScalableFloat>& Pair : DamageTypes)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(1.f);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
		}
		// End of Spec Prep
		
		for(uint32 i = 0; i < NumHits; i++)
		{
			FHitResult ScanHit;
			WeaponTraceHit(Start, HitTarget, ScanHit);
			
			const FVector ToTarget = ScanHit.ImpactPoint - SocketTransform.GetLocation(); // From MuzzleFlashSocket to ImpactLocation
			// const FVector ToTarget = HitTarget - SocketTransform.GetLocation(); // From MuzzleFlashSocket to ImpactLocation
			const FRotator TargetRotation = ToTarget.Rotation();
			// TODO: If Weapon has optional pitch override :
			// Rotation.Pitch = 0.f;

			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SocketTransform.GetLocation());
			SpawnTransform.SetRotation(TargetRotation.Quaternion());
		
			AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
				ProjectileClass,
				SpawnTransform,
				GetOwner(),
				Cast<APawn>(GetOwner()),
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn
				);
			Projectile->DamageEffectSpecHandle = SpecHandle;
			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}
