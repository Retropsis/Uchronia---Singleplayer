// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}
	
	UPROPERTY()
	FGameplayEffectContextHandle EffectContextHandle;	

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	
	UPROPERTY()
	AController* SourceController = nullptr;
	
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	
	UPROPERTY()
	AController* TargetController = nullptr;
	
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

/**
 * 
 */
UCLASS()
class UCHRONIA_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBaseAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/*
	 * Primary Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Strength, Category="Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Strength);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Toughness, Category="Primary Attributes")
	FGameplayAttributeData Toughness;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Toughness);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Dexterity, Category="Primary Attributes")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Dexterity);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Perception, Category="Primary Attributes")
	FGameplayAttributeData Perception;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Perception);
	
	/*
	 * Secondary Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Armor, Category="Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Armor);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ArmorPenetration, Category="Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ArmorPenetration);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BlockChance, Category="Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BlockChance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitChance, Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CriticalHitChance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitDamage, Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CriticalHitDamage);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitResistance, Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CriticalHitResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_HealthRegeneration, Category="Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHunger, Category="Secondary Attributes")
	FGameplayAttributeData MaxHunger;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHunger);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxThirst, Category="Secondary Attributes")
	FGameplayAttributeData MaxThirst;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxThirst);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxBlood, Category="Secondary Attributes")
	FGameplayAttributeData MaxBlood;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxBlood);
	
	/*
	 * Vital Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Hunger, Category="Vital Attributes")
	FGameplayAttributeData Hunger;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Hunger);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Thirst, Category="Vital Attributes")
	FGameplayAttributeData Thirst;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Thirst);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Blood, Category="Vital Attributes")
	FGameplayAttributeData Blood;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Blood);
	
	/*
	 * Resistance Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_FireResistance, Category="Resistance Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, FireResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PhysicalResistance, Category="Resistance Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, PhysicalResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PoisonResistance, Category="Resistance Attributes")
	FGameplayAttributeData PoisonResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, PoisonResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BleedResistance, Category="Resistance Attributes")
	FGameplayAttributeData BleedResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BleedResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_HardResistance, Category="Resistance Attributes")
	FGameplayAttributeData HardResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, HardResistance);

	
	/*
	 * Meta Attributes
	*/
	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, IncomingDamage);

	/*
	 * Replication Notifies
	 */
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	
	UFUNCTION()
	void OnRep_Toughness(const FGameplayAttributeData& OldToughness) const;
	
	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;
	
	UFUNCTION()
	void OnRep_Perception(const FGameplayAttributeData& OldPerception) const;
	
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
	
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;
	
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;
	
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;
	
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;
	
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
	UFUNCTION()
	void OnRep_MaxHunger(const FGameplayAttributeData& OldMaxHunger) const;
	
	UFUNCTION()
	void OnRep_Hunger(const FGameplayAttributeData& OldHunger) const;
	
	UFUNCTION()
	void OnRep_MaxThirst(const FGameplayAttributeData& OldMaxThirst) const;
	
	UFUNCTION()
	void OnRep_Thirst(const FGameplayAttributeData& OldThirst) const;
	
	UFUNCTION()
	void OnRep_MaxBlood(const FGameplayAttributeData& OldMaxBlood) const;
	
	UFUNCTION()
	void OnRep_Blood(const FGameplayAttributeData& OldBlood) const;
	
	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;
	
	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;
	
	UFUNCTION()
	void OnRep_PoisonResistance(const FGameplayAttributeData& OldPoisonResistance) const;
	
	UFUNCTION()
	void OnRep_BleedResistance(const FGameplayAttributeData& OldBleedResistance) const;
	
	UFUNCTION()
	void OnRep_HardResistance(const FGameplayAttributeData& OldHardResistance) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const;
};
