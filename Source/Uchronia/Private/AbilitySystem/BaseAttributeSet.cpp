// Retropsis @ 2023-2024

#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BaseGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "UchroniaBlueprintFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Player/CharacterPlayerController.h"

UBaseAttributeSet::UBaseAttributeSet()
{
}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Toughness, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Perception, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	// TODO: Hunger, Thirst, Blood Variance (Regen/Loss)
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxHunger, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxThirst, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxBlood, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Hunger, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Thirst, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Blood, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, PoisonResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BleedResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, HardResistance, COND_None, REPNOTIFY_Always);
}

// TODO: TagsToAttributes from AttributeMenu Section

// TODO: Investigate PreAttributeBaseChange as it seems it should be this one instead
void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if(Attribute == GetHungerAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHunger());
	}
	if(Attribute == GetThirstAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxThirst());
	}
	if(Attribute == GetBloodAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxBlood());
	}
}

void UBaseAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if(IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if(Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if(const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController =  Pawn->GetController();
			}
		}
		if(Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if(Data.EvaluatedData.Attribute == GetHungerAttribute())
	{
		SetHunger(FMath::Clamp(GetHunger(), 0.f, GetMaxHunger()));
	}
	if(Data.EvaluatedData.Attribute == GetThirstAttribute())
	{
		SetThirst(FMath::Clamp(GetThirst(), 0.f, GetMaxThirst()));
	}
	if(Data.EvaluatedData.Attribute == GetBloodAttribute())
	{
		SetBlood(FMath::Clamp(GetBlood(), 0.f, GetMaxBlood()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if(LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			const bool bFatal = NewHealth <= 0.f;
			if(bFatal)
			{
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Die();
				}
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FBaseGameplayTags::Get().Effects_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
			const bool bBlockedHit = UUchroniaBlueprintFunctionLibrary::IsBlockedHit(Props.EffectContextHandle);
			const bool bCriticalHit = UUchroniaBlueprintFunctionLibrary::IsCriticalHit(Props.EffectContextHandle);
			ShowFloatingText(Props, LocalIncomingDamage, bBlockedHit, bCriticalHit);
		}
	}
}

void UBaseAttributeSet::ShowFloatingText(const FEffectProperties& Props, const float Damage, bool bBlockedHit, bool bCriticalHit) const
{
	if(Props.SourceCharacter != Props.TargetCharacter)
	{
		if (ACharacterPlayerController* PC = Cast<ACharacterPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
			return;
		}
		if (ACharacterPlayerController* PC = Cast<ACharacterPlayerController>(Props.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
		}
	}
}

void UBaseAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Strength, OldStrength);
}

void UBaseAttributeSet::OnRep_Toughness(const FGameplayAttributeData& OldToughness) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Toughness, OldToughness);
}

void UBaseAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Dexterity, OldDexterity);
}

void UBaseAttributeSet::OnRep_Perception(const FGameplayAttributeData& OldPerception) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Perception, OldPerception);
}

void UBaseAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Armor, OldArmor);
}

void UBaseAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UBaseAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BlockChance, OldBlockChance);
}

void UBaseAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UBaseAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UBaseAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UBaseAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldHealth);
}

void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldMaxHealth);
}

void UBaseAttributeSet::OnRep_Hunger(const FGameplayAttributeData& OldHunger) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Hunger, OldHunger);
}

void UBaseAttributeSet::OnRep_MaxHunger(const FGameplayAttributeData& OldMaxHunger) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHunger, OldMaxHunger);
}

void UBaseAttributeSet::OnRep_Thirst(const FGameplayAttributeData& OldThirst) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Thirst, OldThirst);
}

void UBaseAttributeSet::OnRep_MaxThirst(const FGameplayAttributeData& OldMaxThirst) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxThirst, OldMaxThirst);
}

void UBaseAttributeSet::OnRep_Blood(const FGameplayAttributeData& OldBlood) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Blood, OldBlood);
}

void UBaseAttributeSet::OnRep_MaxBlood(const FGameplayAttributeData& OldMaxBlood) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxBlood, OldMaxBlood);
}

void UBaseAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, FireResistance, OldFireResistance);
}

void UBaseAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

void UBaseAttributeSet::OnRep_PoisonResistance(const FGameplayAttributeData& OldPoisonResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, PoisonResistance, OldPoisonResistance);
}

void UBaseAttributeSet::OnRep_BleedResistance(const FGameplayAttributeData& OldBleedResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BleedResistance, OldBleedResistance);
}

void UBaseAttributeSet::OnRep_HardResistance(const FGameplayAttributeData& OldHardResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, HardResistance, OldHardResistance);
}