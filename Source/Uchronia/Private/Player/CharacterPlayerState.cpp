// Retropsis @ 2023-2024


#include "Player/CharacterPlayerState.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Net/UnrealNetwork.h"

ACharacterPlayerState::ACharacterPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSet"));	
	
}

void ACharacterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterPlayerState, Level);
}

void ACharacterPlayerState::OnRep_Level(int32 OldLevel)
{
	
}

UAbilitySystemComponent* ACharacterPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
