// Retropsis @ 2023-2024


#include "UI/Widget/MainMenu.h"

#include "ActorComponents/Inventory/InventoryComponent.h"
#include "ActorComponents/Inventory/ItemBase.h"
#include "Character/PlayerCharacter.h"
#include "UI/Widget/GridInventory.h"
#include "UI/Widget/ItemDragDropOperation.h"

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	GridInventory->InventoryComponent = InventoryComponent;
	// GridInventory->TileSize = TileSize;
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
}

bool UMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if(PlayerCharacter && ItemDragDrop->SourceItem)
	{
		PlayerCharacter->DropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);
		// PlayerCharacter->GetInventory()->DropItem(ItemDragDrop->SourceItem,  ItemDragDrop->SourceItem->Quantity);
		return true;
	}
	return false;
}
