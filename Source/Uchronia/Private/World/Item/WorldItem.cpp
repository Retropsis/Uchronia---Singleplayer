// Retropsis @ 2023-2024

#include "World/Item/WorldItem.h"

AWorldItem::AWorldItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWorldItem::BeginPlay()
{
	Super::BeginPlay();
	InitializePickup(AWorldItem::StaticClass(), ItemQuantity);
}

void AWorldItem::InitializePickup(const TSubclassOf<AWorldItem> BaseClass, const int32 InQuantity)
{
	if(IsValid(ItemDataTable) && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		ItemReference = NewObject<AWorldItem>(this, BaseClass);
		ItemReference->ID = ItemData->ID;
		ItemReference->TextData = ItemData->TextData;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemCondition = ItemData->ItemCondition;
		ItemReference->ItemStatistics = ItemData->ItemStatistics;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->AssetData = ItemData->AssetData;

		ItemReference->NumericData.bIsStackable = ItemData->NumericData.MaxStackSize > 1;
		ItemReference->NumericData.bHasCharges = ItemData->NumericData.MaxCharges > 1;
		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		InteractableMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}

void AWorldItem::InitializeDrop(AWorldItem* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight();
	ItemReference->OverlappingInventory = nullptr;
	PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);
	UpdateInteractableData();
}

void AWorldItem::TryPickup_Implementation(APlayerCharacter* PlayerCharacter)
{
	
}
