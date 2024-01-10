// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

class UInventoryComponent;
class UItemBase;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UItemBase* SourceItem;

	UPROPERTY()
	UInventoryComponent* SourceInventory;
};
