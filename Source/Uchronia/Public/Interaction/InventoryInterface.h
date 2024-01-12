// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

class UItemBase;

UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UCHRONIA_API IInventoryInterface
{
	GENERATED_BODY()

public:
	virtual void TryEquip(UItemBase* ItemToEquip) {};

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
