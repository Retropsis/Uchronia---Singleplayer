// Retropsis @ 2023-2024


#include "BaseGameplayTags.h"
#include "GameplayTagsManager.h"

FBaseGameplayTags FBaseGameplayTags::GameplayTags;

void FBaseGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Primary Attributes
	 */
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"), FString("Physical damage, carry weight, lifting"));
	GameplayTags.Attributes_Primary_Toughness = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Toughness"), FString("Physical resistance, health and carry weight"));
	GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Dexterity"), FString("Skill in performing tasks, especially with hands"));
	GameplayTags.Attributes_Primary_Perception = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Perception"), FString("Ability to see, hear or become aware of something through the senses"));

	/*
	 * Secondary Attributes
	 */
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage taken and improves block chance"));
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"), FString("Armor reduction"));
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"), FString("Chance to block a hit"));
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"), FString("Chance to apply a critical hit"));
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"), FString("Critical damage amount"));
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"), FString("Reduces critical damage"));
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"), FString("Amount of health points regenerated periodically"));
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString("Amount of maximum health points"));
	GameplayTags.Attributes_Secondary_MaxHunger = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHunger"), FString("Amount of maximum hunger points"));
	GameplayTags.Attributes_Secondary_MaxThirst = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxThirst"), FString("Amount of maximum thirst points"));
	GameplayTags.Attributes_Secondary_MaxBlood = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxBlood"), FString("Amount of maximum blood points"));

	/*
	 * Vital Attributes
	 */
	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"), FString("Amount of health points"));
	GameplayTags.Attributes_Vital_Hunger = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Hunger"), FString("Amount of hunger points"));
	GameplayTags.Attributes_Vital_Thirst = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Thirst"), FString("Amount of thirst points"));
	GameplayTags.Attributes_Vital_Blood = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Blood"), FString("Amount of blood points"));

	/*
	 * Resistances
	 */
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"), FString("Physical Resistance"));
	GameplayTags.Attributes_Resistance_Hard = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Hard"), FString("Hard Resistance"));
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString("Fire Resistance"));
	GameplayTags.Attributes_Resistance_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Poison"), FString("Poison Resistance"));
	GameplayTags.Attributes_Resistance_Bleed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Bleed"), FString("Bleed Resistance"));
	
	/*
	 * Input Tags for Keybinding
	 */
	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"), FString("Amount of health points"));
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("InputTag for Left Mouse Button"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("InputTag for Right Mouse Button"));
	GameplayTags.InputTag_MMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.MMB"), FString("InputTag for Middle Mouse Button"));
	GameplayTags.InputTag_Space = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Space"), FString("InputTag for Space Bar"));
	GameplayTags.InputTag_0 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.0"), FString("InputTag.0"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString("InputTag.1"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString("InputTag.2"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), FString("InputTag.3"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), FString("InputTag.4"));
	GameplayTags.InputTag_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.5"), FString("InputTag.5"));
	GameplayTags.InputTag_6 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.6"), FString("InputTag.6"));
	GameplayTags.InputTag_7 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.7"), FString("InputTag.7"));
	GameplayTags.InputTag_8 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.8"), FString("InputTag.8"));
	GameplayTags.InputTag_9 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.9"), FString("InputTag.9"));
	GameplayTags.InputTag_E = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.E"), FString("InputTag.E"));
	GameplayTags.InputTag_F = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.F"), FString("InputTag.F"));
	GameplayTags.InputTag_G = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.G"), FString("InputTag.G"));
	GameplayTags.InputTag_D = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.D"), FString("InputTag.D"));
	GameplayTags.InputTag_R = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.R"), FString("InputTag.R"));
	GameplayTags.InputTag_T = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.T"), FString("InputTag.T"));
	GameplayTags.InputTag_C = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.C"), FString("InputTag.C"));
	GameplayTags.InputTag_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Q"), FString("InputTag.Q"));
	GameplayTags.InputTag_LShift = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LShift"), FString("InputTag.LShift"));
	GameplayTags.InputTag_LCtrl = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LCtrl"), FString("InputTag.LCtrl"));
	GameplayTags.InputTag_LAlt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LAlt"), FString("InputTag.LAlt"));

	/*
	 * Damage Types
	 */
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage"));
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), FString("Fire Damage"));
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString("Physical Damage"));
	GameplayTags.Damage_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Poison"), FString("Poison Damage Type"));
	GameplayTags.Damage_Bleed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Bleed"), FString("Bleed Damage Type"));
	GameplayTags.Damage_Hard = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Hard"), FString("Damage to machines"));

	//~ DamageTypesToResistances
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Poison, GameplayTags.Attributes_Resistance_Poison);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Bleed, GameplayTags.Attributes_Resistance_Bleed);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Hard, GameplayTags.Attributes_Resistance_Hard);
	
	/*
	 * Effects
	 */
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Effect HitReact"));
	
	/*
	 * Abilities
	 */
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"), FString("Melee Attack Ability"));
	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Summon"), FString("Summoning Ability"));
	GameplayTags.Abilities_FirstAid = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.FirstAid"), FString("First Aid Ability"));
	
	/*
	 * Combat Sockets
	 */
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"), FString("CombatSocket Weapon"));
	GameplayTags.CombatSocket_Hand_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Hand.Left"), FString("CombatSocket Hand Left"));
	GameplayTags.CombatSocket_Hand_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Hand.Right"), FString("CombatSocket Hand Right"));
	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Tail"), FString("CombatSocket Tail"));
	GameplayTags.CombatSocket_Head = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Head"), FString("CombatSocket Head"));
	GameplayTags.CombatSocket_Horn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Horn"), FString("CombatSocket Horn"));
	GameplayTags.CombatSocket_Foot_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Foot.Left"), FString("CombatSocket Foot Left"));
	GameplayTags.CombatSocket_Foot_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Foot.Right"), FString("CombatSocket Foot Right"));
	
	/*
	 * MontagesSockets
	 */
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"), FString("Montage Attack 1"));
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"), FString("Montage Attack 2"));
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"), FString("Montage Attack 3"));
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"), FString("Montage Attack 4"));
	
	/*
	 * Interaction Montages
	 */
	GameplayTags.Interaction_Sitting_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Interaction.Sitting.1"), FString("Interaction.Sitting.1"));
	GameplayTags.Interaction_Sitting_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Interaction.Sitting.2"), FString("Interaction.Sitting.2"));
	GameplayTags.Interaction_Leaning_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Interaction.Leaning.1"), FString("Interaction.Leaning.1"));
	GameplayTags.Interaction_Leaning_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Interaction.Leaning.2"), FString("Interaction.Leaning.2"));
	GameplayTags.Interaction_Carrying_Light = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Interaction.Carrying.Light"), FString("Interaction.Carrying.Light"));
	GameplayTags.Interaction_Carrying_Average = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Interaction.Carrying.Average"), FString("Interaction.Carrying.Average"));
	GameplayTags.Interaction_Carrying_Heavy = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Interaction.Carrying.Heavy"), FString("Interaction.Carrying.Heavy"));
	GameplayTags.Interaction_Chest_Opening = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Interaction.Chest.Opening"), FString("Interaction.Chest.Opening"));
	GameplayTags.Interaction_Chest_Closing = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Interaction.Chest.Closing"), FString("Interaction.Chest.Closing"));
}
