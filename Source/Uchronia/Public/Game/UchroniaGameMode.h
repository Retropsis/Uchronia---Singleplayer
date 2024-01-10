// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UchroniaGameMode.generated.h"

class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class UCHRONIA_API AUchroniaGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
};
