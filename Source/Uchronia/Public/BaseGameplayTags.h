// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 *  Singleton containing Native Gameplay Tags
 */
struct FBaseGameplayTags
{
public:
 static const FBaseGameplayTags& Get() { return GameplayTags; }
 static void InitializeNativeGameplayTags();

 FGameplayTag Attributes_Primary_Strength;
 FGameplayTag Attributes_Primary_Toughness;
 FGameplayTag Attributes_Primary_Dexterity;
 FGameplayTag Attributes_Primary_Perception;
 
 FGameplayTag Attributes_Secondary_Armor;
 FGameplayTag Attributes_Secondary_ArmorPenetration;
 FGameplayTag Attributes_Secondary_BlockChance;
 FGameplayTag Attributes_Secondary_CriticalHitChance;
 FGameplayTag Attributes_Secondary_CriticalHitDamage;
 FGameplayTag Attributes_Secondary_CriticalHitResistance;
 FGameplayTag Attributes_Secondary_HealthRegeneration;
 FGameplayTag Attributes_Secondary_MaxHealth;
 FGameplayTag Attributes_Secondary_MaxHunger;
 FGameplayTag Attributes_Secondary_MaxThirst;
 FGameplayTag Attributes_Secondary_MaxBlood;
 
 FGameplayTag Attributes_Resistance_Physical;
 FGameplayTag Attributes_Resistance_Fire;
 FGameplayTag Attributes_Resistance_Hard;
 FGameplayTag Attributes_Resistance_Poison;
 FGameplayTag Attributes_Resistance_Bleed;
 
 FGameplayTag Attributes_Vital_Health;
 FGameplayTag Attributes_Vital_Hunger;
 FGameplayTag Attributes_Vital_Thirst;
 FGameplayTag Attributes_Vital_Blood;

 FGameplayTag InputTag_LMB;
 FGameplayTag InputTag_RMB;
 FGameplayTag InputTag_MMB;
 FGameplayTag InputTag_Space;
 FGameplayTag InputTag_0;
 FGameplayTag InputTag_1;
 FGameplayTag InputTag_2;
 FGameplayTag InputTag_3;
 FGameplayTag InputTag_4;
 FGameplayTag InputTag_5;
 FGameplayTag InputTag_6;
 FGameplayTag InputTag_7;
 FGameplayTag InputTag_8;
 FGameplayTag InputTag_9;
 FGameplayTag InputTag_E;
 FGameplayTag InputTag_F;
 FGameplayTag InputTag_G;
 FGameplayTag InputTag_D;
 FGameplayTag InputTag_R;
 FGameplayTag InputTag_T;
 FGameplayTag InputTag_C;
 FGameplayTag InputTag_Q;
 FGameplayTag InputTag_LShift;
 FGameplayTag InputTag_LCtrl;
 FGameplayTag InputTag_LAlt;
 
 FGameplayTag Effects_HitReact;

 /*
  * Damage Types
  */
 FGameplayTag Damage;
 FGameplayTag Damage_Fire;
 FGameplayTag Damage_Poison;
 FGameplayTag Damage_Bleed;
 FGameplayTag Damage_Physical;
 FGameplayTag Damage_Hard;

 /*
  * Abilities
  */
 FGameplayTag Abilities_Attack;
 FGameplayTag Abilities_Summon;
 FGameplayTag Abilities_FirstAid;

 /*
  * CombatSockets
  */
 FGameplayTag CombatSocket_Weapon;
 FGameplayTag CombatSocket_Hand_Left;
 FGameplayTag CombatSocket_Hand_Right;
 FGameplayTag CombatSocket_Tail;
 FGameplayTag CombatSocket_Head;
 FGameplayTag CombatSocket_Horn;
 FGameplayTag CombatSocket_Foot_Left;
 FGameplayTag CombatSocket_Foot_Right;

 /*
  * Montage
  */
 FGameplayTag Montage_Attack_1;
 FGameplayTag Montage_Attack_2;
 FGameplayTag Montage_Attack_3;
 FGameplayTag Montage_Attack_4;
 FGameplayTag Montage_Attack_5;
 
 TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
 
protected:

private:
 static  FBaseGameplayTags GameplayTags;
};
