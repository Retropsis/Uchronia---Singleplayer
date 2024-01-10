// Retropsis @ 2023-2024

#include "UI/Widget/InventoryWidget.h"
#include "ActorComponents/Inventory/InventoryComponent.h"
#include "Character/PlayerCharacter.h"
#include "Components/WrapBox.h"
#include "UI/Widget/InventorySlot.h"
#include "World/WorldItem_.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventoryComponent();
	}
	
}

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventoryWidget::UpdateInventory()
{
	checkf(InventorySlotClass, TEXT("InventorySlotClass is missing, please fill up InventoryPanel!"));
	PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayer()->GetCharacter());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventoryComponent();
	}
	if(InventoryReference)
	{
		InventoryWrapBox->ClearChildren();
		for(const TSubclassOf<AWorldItem_>& InventoryItem : InventoryReference->GetInventory_())
		{
			UInventorySlot* ItemSlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
			// Test here GetClass
			ItemSlot->SetItemReference(InventoryItem->GetDefaultObject<AWorldItem_>());
			InventoryWrapBox->AddChildToWrapBox(ItemSlot);
		}
	}
}
