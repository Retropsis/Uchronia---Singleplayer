// Retropsis @ 2023-2024


#include "UI/Widget/InventoryToolTip_.h"

#include "Components/TextBlock.h"
#include "UI/Widget/InventorySlot.h"
#include "World/WorldItem_.h"

void UInventoryToolTip_::NativeConstruct()
{
	Super::NativeConstruct();

	const AWorldItem_* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();
	ItemName->SetText(ItemBeingHovered->ItemName);
	ItemDescription->SetText(ItemBeingHovered->ItemDescription);
}
