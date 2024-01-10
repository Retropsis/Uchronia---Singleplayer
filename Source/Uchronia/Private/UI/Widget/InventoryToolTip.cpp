// Retropsis @ 2023-2024

#include "UI/Widget/InventoryToolTip.h"
#include "UI/Widget/InventoryItemSlot.h"
#include "ActorComponents/Inventory/ItemBase.h"

#include "Components/TextBlock.h"

void UInventoryToolTip::NativeConstruct()
{
	Super::NativeConstruct();

	const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

	switch (ItemBeingHovered->ItemType) {
	case EItemType::EIT_Weapon:
		break;
	case EItemType::EIT_Armor:
		break;
	case EItemType::EIT_Ammunition:
		break;
	case EItemType::EIT_Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		// Collapse here unneeded info
		break;
	case EItemType::EIT_Material:
		break;
	case EItemType::EIT_Misc:
		ItemType->SetText(FText::FromString("Misc."));
		ItemUsage->SetVisibility(ESlateVisibility::Collapsed);
		// Collapse here unneeded info
		
		break;
	default: ;
	}
	ItemName->SetText(ItemBeingHovered->TextData.Name);
	ItemDescription->SetText(ItemBeingHovered->TextData.Description);
	ItemUsage->SetText(ItemBeingHovered->TextData.Usage);
	
	const FString WeightInfo = { "Weight: " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight()) };
	StackWeight->SetText(FText::FromString(WeightInfo));
	
	if(ItemBeingHovered->NumericData.bIsStackable)
	{
		const FString StackInfo = { "Max Stack Size: " + FString::FromInt(ItemBeingHovered->NumericData.MaxStackSize) };
		MaxStackSize->SetText(FText::FromString(StackInfo));
	}
	else
	{
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
}
