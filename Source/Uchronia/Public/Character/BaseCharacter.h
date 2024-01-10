// Retropsis @ 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/CrosshairInterface.h"
#include "AbilitySystemInterface.h"
#include "Interaction/SwimmingInterface.h"
#include "BaseCharacter.generated.h"

class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class UCHRONIA_API ABaseCharacter : public ACharacter, public ICrosshairInterface, public ICombatInterface, public IAbilitySystemInterface, public ISwimmingInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	/* Combat Interface */
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual void HitReact(const FVector& ImpactPoint) override;
	virtual void Die() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetSoftBodyImpact_Implementation() override;
	virtual USoundBase* GetSoftBodySound_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncrementMinionCount_Implementation(int32 Amount) override;
	/* Combat Interface */
	
	//~Swimming Interface
	virtual void SetSwimmingState_Implementation(bool bShouldSwim) override;
	//~Swimming Interface

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	
	UPROPERTY(EditDefaultsOnly, Category="Base Character Properties|Montages")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category="Base Character Properties|Montages")
	TArray<FTaggedMontage> AttackMontages;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category="Base Character Properties")
	float DespawnLifeSpan = 3.f;

	bool bDead = false;
	
	/*
	 * Ability System
	 */
	virtual void InitAbilityActorInfo();
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual  void InitializeDefaultAttributes() const;
	void AddCharacterAbilities();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Base Character Properties|Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Base Character Properties|Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Base Character Properties|Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	// TODO: Might need to be more generic
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Character Properties|Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category="Base Character Properties|Combat")
	TObjectPtr<UStaticMeshComponent> ThrowableItem;

	UPROPERTY(EditAnywhere, Category="Base Character Properties|Combat")
	FName ThrownItemSocketName;
	
	UPROPERTY(EditAnywhere, Category="Base Character Properties|Combat")
	FName CombatSocketName;
	
	UPROPERTY(EditAnywhere, Category="Base Character Properties|Combat")
	FName LeftHandSocketName;
	
	UPROPERTY(EditAnywhere, Category="Base Character Properties|Combat")
	FName RightHandSocketName;
	
	UPROPERTY(EditAnywhere, Category="Base Character Properties|Combat")
	FName TailSocketName;

	/*
	 * Minions
	 */
	int32 MinionCount = 0;

	/*
	 * Swimming
	 */
	bool bSwimming = false;

	/*
	 * Dissolve Effect
	 */
	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Character Properties|Dissolve")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Character Properties|Dissolve")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	/*
	 *  SFX & VFX
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Character Properties|Combat|VFX")
	UNiagaraSystem* SoftBodyImpact;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Character Properties|Combat|SFX")
	USoundBase* SoftBodySound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base Character Properties|Combat|SFX")
	USoundBase* DeathSound;	
	/*
	 * SFX & VFX - END
	 */

private:
	UPROPERTY(EditAnywhere, Category="Base Character Properties|Combat|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

public:	
	FORCEINLINE UStaticMeshComponent* GetThrowableItem() const { return ThrowableItem; }

	UFUNCTION(BlueprintCallable)
	bool IsSwimming() const { return bSwimming; }
};
