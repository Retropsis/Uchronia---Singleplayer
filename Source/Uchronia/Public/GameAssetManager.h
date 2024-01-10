// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GameAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class UCHRONIA_API UGameAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UGameAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
