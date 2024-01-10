// Retropsis @ 2023-2024


#include "Input/InputConfig.h"

const UInputAction* UInputConfig::FindKeybindInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for(const FBaseKeybind& Action : Keybinds)
	{
		if(Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Find KeybindInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
