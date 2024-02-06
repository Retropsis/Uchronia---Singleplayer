// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Actor/Weapon/WeaponData.h"
#include "UObject/Interface.h"
#include "LootInterface.generated.h"

UINTERFACE(MinimalAPI)
class ULootInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UCHRONIA_API ILootInterface
{
	GENERATED_BODY()

public:
	virtual void LootAmmunition(EWeaponType WeaponType, int32 Amount) = 0;
};
