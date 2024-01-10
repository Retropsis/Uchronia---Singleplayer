// Retropsis @ 2023-2024

#include "UI/Widget/DropSlot.h"
#include "Character/PlayerCharacter.h"
#include "UI/Widget/ItemDragDropOperation_.h"
#include "World/WorldItem_.h"

void UDropSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UDropSlot::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
}

bool UDropSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UItemDragDropOperation_* ItemDragDrop = Cast<UItemDragDropOperation_>(InOperation);

	if(PlayerCharacter && ItemDragDrop->SourceItem/* && ItemDragDrop->SourceInventory*/)
	{
		// Drop here
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Emerald, FString::Printf(TEXT("Dropped: %s"), *ItemDragDrop->SourceItem->GetName()));
		// Do this without interface, it was working
		// PlayerCharacter->ServerSpawnIem(ItemDragDrop->SourceItem->GetClass(), FTransform());
		// Here with interface, working too
		if(IInteractionInterface* Interface = Cast<IInteractionInterface>(PlayerCharacter))
		{
			Interface->DropItem(ItemDragDrop->SourceItem->GetClass());
		}
		// 3rd try from InventoryComponent Interfacing, Source inventory seems always null ???
		// if(IInteractionInterface* Interface = Cast<IInteractionInterface>(ItemDragDrop->SourceInventory))
		// {
		// 	Interface->SpawnItem(ItemDragDrop->SourceItem->GetClass());
		// }
		
		// ItemDragDrop->SourceInventory->ServerSpawnActorFromClass(ItemDragDrop->SourceItem->GetClass(), FTransform());
		// ServerSpawnActorFromClass(ItemDragDrop->SourceItem->GetClass(), FTransform());
		return true;
	}
	return false;
}
