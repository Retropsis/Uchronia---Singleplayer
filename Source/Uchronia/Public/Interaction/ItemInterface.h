// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Actor/Weapon/AmmunitionData.h"
#include "Inventory/Data/ItemData.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UCHRONIA_API IItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsAmmoContainer();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsAmmunition();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsWeapon();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetCurrentCount();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetMaxCapacity();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetAvailableSpace();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsEmpty();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsFull();

	// Returns the left over
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 RefillContainer(int32 AvailableAmount);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCount(int32 NewAmount);

	// Returns how many it contained
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 EmptyContainer();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EAmmunitionType GetAmmunitionType();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EAmmoContainerType GetAmmoContainerType();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ESlotType GetSlotType();
};
