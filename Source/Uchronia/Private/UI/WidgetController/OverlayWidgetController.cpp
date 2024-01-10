// Retropsis @ 2023-2024

#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Player/CharacterPlayerController.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UBaseAttributeSet* BaseAttributeSet = CastChecked<UBaseAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(BaseAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(BaseAttributeSet->GetMaxHealth());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UBaseAttributeSet* BaseAttributeSet = CastChecked<UBaseAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
	if(ACharacterPlayerController* CharacterPlayerController = Cast<ACharacterPlayerController>(PlayerController))
	{
		CharacterPlayerController->OnAmmoAmountChanged.AddLambda(
			[this](int32 NewCount)
			{
				OnWeaponAmmoAmountChanged.Broadcast(NewCount);
			});
		CharacterPlayerController->OnCarriedAmmoAmountChanged.AddLambda(
			[this](int32 NewCount)
			{
				OnWeaponCarriedAmmoChanged.Broadcast(NewCount);
			});
		CharacterPlayerController->OnGrenadeAmountChanged.AddLambda(
			[this](int32 NewCount)
			{
				OnGrenadeCountChanged.Broadcast(NewCount);
			});
	}

	Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for(const FGameplayTag& Tag : AssetTags)
			{
				//~ "Message.WaterBottle".MatchesTag("Message") will return True, "Message".MatchesTag("Message.WaterBottle") will return False
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);
}
