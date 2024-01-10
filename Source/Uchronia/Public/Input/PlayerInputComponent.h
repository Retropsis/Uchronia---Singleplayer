// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputConfig.h"
#include "PlayerInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class UCHRONIA_API UPlayerInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void SetupKeybindInputActions(const UInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
	
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UPlayerInputComponent::SetupKeybindInputActions(const UInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);
	for(const FBaseKeybind& Keybind : InputConfig->Keybinds)
	{
		if(Keybind.InputAction && Keybind.InputTag.IsValid())
		{
			if(PressedFunc)
			{
				BindAction(Keybind.InputAction, ETriggerEvent::Started, Object, PressedFunc, Keybind.InputTag);
			}
			if(ReleasedFunc)
			{
				BindAction(Keybind.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Keybind.InputTag);
			}
			if(HeldFunc)
			{
				BindAction(Keybind.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Keybind.InputTag);
			}
		}
	}
}
