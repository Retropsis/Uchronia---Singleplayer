// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UGameModeBase.generated.h"

class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class UCHRONIA_API AUGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};
