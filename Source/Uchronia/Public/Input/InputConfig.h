// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FBaseKeybind
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};


/**
 * 
 */
UCLASS()
class UCHRONIA_API UInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindKeybindInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FBaseKeybind> Keybinds;
};
