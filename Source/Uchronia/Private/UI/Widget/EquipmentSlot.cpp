// Retropsis @ 2023-2024


#include "UI/Widget/EquipmentSlot.h"

#include "Actor/Weapon/Weapon.h"
#include "ActorComponents/CombatComponent.h"
#include "ActorComponents/Inventory/InventoryComponent.h"
#include "ActorComponents/Inventory/ItemBase.h"
#include "Character/PlayerCharacter.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "UI/Widget/ItemDragDropOperation.h"

void UEquipmentSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

bool UEquipmentSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);
	if(ItemDragDrop->SourceItem)
	{
		GEngine->AddOnScreenDebugMessage(50, 5.f, FColor::Emerald, FString::Printf(TEXT("Item Drop on Equipment Slot")));
		
		if(ItemDragDrop->SourceItem->ItemType == SlotType)
		{
			ItemReference = ItemDragDrop->SourceItem;
			PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn());
			if (PlayerCharacter)
			{
				if(IInventoryInterface* Interface = Cast<IInventoryInterface>(ItemDragDrop->SourceInventory))
				{
					Interface->TryEquip(ItemDragDrop->SourceItem);
				}
			}
			RefreshSlot();
		}
		// return true will stop  the drop operation at this widget
		return true;
	}
	// return false  will cause the drop operation  to fall through underlying widgets if any
	return false;
}

void UEquipmentSlot::RefreshSlot() const
{
	if(ItemReference && ItemIcon)
	{
		ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
		ItemIcon->SetColorAndOpacity(FLinearColor(FLinearColor(1.f, 1.f, 1.f, 1.f)));
		switch (ItemReference->ItemCondition)
		{
		case EItemCondition::EIC_Pristine:
			ItemBorder->SetBrushColor(FLinearColor::Green);
			break;
		case EItemCondition::EIC_Worn:
			ItemBorder->SetBrushColor(FLinearColor(0.f, 1.f, 0.45f));
			break;
		case EItemCondition::EIC_Damaged:
			ItemBorder->SetBrushColor(FLinearColor::Yellow);
			break;
		case EItemCondition::EIC_BadlyDamaged:
			ItemBorder->SetBrushColor(FLinearColor(1.f, 0.45f, 0.f));
			break;
		case EItemCondition::EIC_Ruined:
			ItemBorder->SetBrushColor(FLinearColor::Red);
			break;
		default: ;
		}
	}
}
