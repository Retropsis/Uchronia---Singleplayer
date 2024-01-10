// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation_.generated.h"

class UInventoryComponent;
class AWorldItem_;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UItemDragDropOperation_ : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TObjectPtr<AWorldItem_> SourceItem;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> SourceInventory;
};
