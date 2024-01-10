// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SwimmingInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class USwimmingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UCHRONIA_API ISwimmingInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetSwimmingState(bool bShouldSwim);
};
