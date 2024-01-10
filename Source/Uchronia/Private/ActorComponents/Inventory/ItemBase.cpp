// Retropsis @ 2023-2024


#include "ActorComponents/Inventory/ItemBase.h"
#include "ActorComponents/Inventory/InventoryComponent.h"

UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false)
{
}

void UItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemCondition = this->ItemCondition;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->TextData = this->TextData;
	ItemCopy->AssetData = this->AssetData;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

void UItemBase::SetQuantity(int32 NewQuantity)
{
	if(NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize : 1);

		 if(OwningInventory)
		 {
		 	if(Quantity <= 0)
		 	{
					OwningInventory->RemoveSingleInstanceOfItem(this);
		     }
		 }
	}
}

void UItemBase::Use(APlayerCharacter* PlayerCharacter)
{
}
