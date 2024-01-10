// Retropsis @ 2023-2024


#include "UI/Widget/InventoryPanel.h"

#include "ActorComponents/Inventory/InventoryComponent.h"
#include "ActorComponents/Inventory/ItemBase.h"
#include "Character/PlayerCharacter.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "UI/Widget/InventoryItemSlot.h"
#include "UI/Widget/ItemDragDropOperation.h"

void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
		if(InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory);
			SetInfoText();
		}
	}
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);
	if(ItemDragDrop->SourceItem && InventoryReference)
	{
		GEngine->AddOnScreenDebugMessage(50, 5.f, FColor::Emerald, FString::Printf(TEXT("Item Drop on Inventory Panel")));

		// return true will stop  the drop operation at this widget
		return true;
	}
	// return false  will cause the drop operation  to fall through underlying widgets if any
	return false;
}

void UInventoryPanel::RefreshInventory()
{
	checkf(InventorySlotClass, TEXT("InventorySlotClass is missing, please fill up InventoryPanel!"));
	if(InventoryReference)
	{
		InventoryWrapBox->ClearChildren();
		for(UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);
			
			InventoryWrapBox->AddChildToWrapBox(ItemSlot);
		}
		SetInfoText();
	}
}

void UInventoryPanel::SetInfoText() const
{
	const FString WeightInfoValue = { FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight())  + "/" + FString::SanitizeFloat(InventoryReference->GetWeightCapacity()) };
	WeightInfo->SetText(FText::FromString(WeightInfoValue));
	
	const FString CapacityInfoValue = { FString::FromInt(InventoryReference->GetInventoryContents().Num())  + "/" + FString::FromInt(InventoryReference->GetSlotsCapacity()) };
	CapacityInfo->SetText(FText::FromString(CapacityInfoValue));
}
