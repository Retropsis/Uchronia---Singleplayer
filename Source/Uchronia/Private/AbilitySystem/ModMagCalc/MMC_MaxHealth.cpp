// Retropsis @ 2023-2024


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/BaseAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	ToughnessDef.AttributeToCapture = UBaseAttributeSet::GetToughnessAttribute();
	ToughnessDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ToughnessDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(ToughnessDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Toughness = 0.f;
	GetCapturedAttributeMagnitude(ToughnessDef, Spec, EvaluationParameters, Toughness);
	Toughness = FMath::Max<float>(Toughness, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 CharacterLevel = CombatInterface->GetCharacterLevel();

	// TODO: Make it reach 100.f for humanoid Level 1
	return 70.f + 2.f * Toughness + 10.f * CharacterLevel;
}
