// Retropsis @ 2023-2024

#include "ActorComponents/Inventory/InventoryComponent.h"
#include "Actor/Weapon/Weapon.h"
#include "ActorComponents/CombatComponent.h"
#include "ActorComponents/Inventory/ItemBase.h"
#include "Item/Pickup.h"
#include "Net/UnrealNetwork.h"
#include "World/GridItem/ItemData_.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, EquippedMainHand);
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	// if(PlayerCharacter)
	// {
	// 	PlayerCharacter->SetInventoryComponent(this);
	// }
	Items.Init(nullptr, GridColumns * GridRows);
}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* InItem) const
{
	if(InItem)
	{
		if(InventoryContents.Contains(InItem))
		{
			return InItem;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* InItem) const
{
	if(InItem)
	{
		if(const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(InItem))
		{
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindItemByID(FName InItemID) const
{
	if(!InItemID.IsNone())
	{
		for(TObjectPtr<UItemBase> Item : InventoryContents)
		{
			if(Item.Get()->ID == InItemID)
			{
				return Item.Get();
			}
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* InItem) const
{
	if(const TArray<TObjectPtr<UItemBase>>::ElementType* Result =
		InventoryContents.FindByPredicate([&InItem] (const UItemBase* InventoryItem)
		{
			return InventoryItem->ID == InItem->ID && !InventoryItem->IsItemFullStack();
		}))
	{
		return *Result;
	}
	return nullptr;
}

int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase* InItem, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / InItem->GetItemSingleWeight());
	if(WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemToRemove, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemToRemove->Quantity);
	ItemToRemove->SetQuantity(ItemToRemove->Quantity - ActualAmountToRemove);
	InventoryTotalWeight -= ActualAmountToRemove * ItemToRemove->GetItemSingleWeight();
	OnInventoryUpdated.Broadcast();
	return ActualAmountToRemove;
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemToSplit, int32 AmountToSplit)
{
	if (!(InventoryContents.Num() + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(ItemToSplit, AmountToSplit);
		AddNewItem(ItemToSplit, AmountToSplit);
	}
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* ItemToAdd)
{
	if(GetOwner()) //GetOwnerRole() might need this one?
	{
		const int32 InitialRequestedAddAmount = ItemToAdd->Quantity;

		// Non Stackable Items
		if(!ItemToAdd->NumericData.bIsStackable)
		{
			return HandleNonStackableItems(ItemToAdd);
		}

		// Stackable Items
		const int32 StackableAmountAdded =	HandleStackableItems(ItemToAdd, InitialRequestedAddAmount);
		if(StackableAmountAdded == InitialRequestedAddAmount)
		{
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(FText::FromString("{0} {1} was added to the inventory."), ItemToAdd->TextData.Name));
		}
		if(StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(FText::FromString("Partial amount of {0} was added to inventory. Amount = {1}"), ItemToAdd->TextData.Name));
		}
		if(StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. Item is invalid or no available slot."), ItemToAdd->TextData.Name));
		}
	}
	return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add to the inventory. GetOwner() returned null"), ItemToAdd->TextData.Name));
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* InItem)
{
	// Check for valid weight
	if(FMath::IsNearlyZero(InItem->GetItemSingleWeight()) || InItem->GetItemStackWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. Item has invalid weight value."), InItem->TextData.Name));
	}

	// Check if new Item will overflow weight capacity
	if(InventoryTotalWeight + InItem->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. Item would overflow weight capacity."), InItem->TextData.Name));
	}

	// Check if new Item will overflow slots capacity
	if(InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. Inventory is full."), InItem->TextData.Name));
	}
	AddNewItem(InItem, 1);
	return FItemAddResult::AddedAll(1, FText::Format(FText::FromString("{0} was added to the inventory."), InItem->TextData.Name));
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* InItem, int32 RequestedAddAmount)
{
	// The function tries to distribute the stack within existing available stack of this same item first, within weight capacity
	// Then eventually make new stacks within inventory slot capacity and weight capacity
	if(RequestedAddAmount <= 0 || FMath::IsNearlyZero(InItem->GetItemStackWeight()))
	{
		return 0;
	}
	int32 AmountToDistribute = RequestedAddAmount;

	// Check for existing available stacsk of this same item to fill up
	UItemBase* ExistingItemStack = FindNextPartialStack(InItem);

	// Distribute item stack over existing stacks
	while (ExistingItemStack)
	{
		// How much we can add to this stack to make it full, and then how many within weight capacity
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);
		
		if(WeightLimitAddAmount > 0)
		{
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount;
			AmountToDistribute -= WeightLimitAddAmount;
			InItem->SetQuantity(AmountToDistribute);

			// Max weight is reached, no more item can be added
			// if(InventoryTotalWeight >= InventoryWeightCapacity)
			if(InventoryTotalWeight + ExistingItemStack->GetItemSingleWeight() > InventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
		}
		else if(WeightLimitAddAmount <= 0)
		{
			if(AmountToDistribute != RequestedAddAmount)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
			return 0;
		}
		if(AmountToDistribute <= 0)
		{
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}
		ExistingItemStack = FindNextPartialStack(InItem);
	}

	// No more partial stack found, make a new stack if possible
	if(InventoryContents.Num() + 1 <= InventorySlotsCapacity)
	{
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(InItem, AmountToDistribute);
		if(WeightLimitAddAmount > 0)
		{
			if(WeightLimitAddAmount < AmountToDistribute)
			{
				AmountToDistribute -= WeightLimitAddAmount;
				InItem->SetQuantity(AmountToDistribute);
				AddNewItem(InItem->CreateItemCopy(), WeightLimitAddAmount);
				return RequestedAddAmount - AmountToDistribute;
			}
			AddNewItem(InItem, AmountToDistribute);
			return RequestedAddAmount;
		}
		return RequestedAddAmount - AmountToDistribute;
	}
	return 0;
}

void UInventoryComponent::AddNewItem(UItemBase* Item, int32 AmountToAdd)
{
	UItemBase* NewItem;

	if(Item->bIsCopy || Item->bIsPickup)
	{
		// when item is already a copy or from world pickup
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// when item is splitting pr dragged to another inventory
		NewItem = Item->CreateItemCopy();
	}
	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
}

// TODO: Probably the wrong way to do this
void UInventoryComponent::ServerDestroyPickup_Implementation(APickup* PickupToDestroy)
{
	PickupToDestroy->Destroy();
}

void UInventoryComponent::DropAllInventory()
{
	if(GetOwner()->HasAuthority())
	{
		for(UItemBase* Item : InventoryContents)
		{
			DropItem(Item, Item->Quantity);
		}
	}
}

void UInventoryComponent::DropItem(UItemBase* ItemToDrop, const int32 Quantity)
{
	if(GetOwner()->HasAuthority())
	{
		if (PlayerCharacter)
		{
			PlayerCharacter->DropItem(ItemToDrop, Quantity);
		}
	}
}

/*
* Equipment
*/
void UInventoryComponent::TryEquip(UItemBase* ItemToEquip, const EItemType SlotType)
{
	switch (SlotType)
	{
	case EItemType::EIT_Weapon:
		TryEquipWeapon(ItemToEquip);
		break;
	case EItemType::EIT_Armor:
		break;
	case EItemType::EIT_Ammunition:
		break;
	case EItemType::EIT_Consumable:
		break;
	case EItemType::EIT_Material:
		break;
	case EItemType::EIT_Misc:
		break;
	default: ;
	}
}

void UInventoryComponent::TryEquip(UItemBase* ItemToEquip)
{
	TryEquip(ItemToEquip, EItemType::EIT_Weapon);
}

void UInventoryComponent::TryEquipItem()
{
}

void UInventoryComponent::TryEquipWeapon(UItemBase* ItemToEquip)
{
	if(AWeapon* WeaponToEquip = Cast<AWeapon>(ItemToEquip->AssetData.Weapon))
	{
		PlayerCharacter->GetCombatComponent()->EquipWeapon(WeaponToEquip);
	}
}

void UInventoryComponent::ClientSetEquippedMainHand_Implementation(UClass* ItemToEquip)
{
	EquippedMainHand = ItemToEquip;
	ServerSetEquippedMainHand(ItemToEquip);
}

void UInventoryComponent::ServerSetEquippedMainHand_Implementation(UClass* ItemToEquip)
{
	EquippedMainHand = ItemToEquip;
}

void UInventoryComponent::OnRep_EquippedMainHand()
{
	if (PlayerCharacter)
	{
		// const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
		// PlayerCharacter->GetMesh()->AttachToComponent(PlayerCharacter->GetMesh(), TransformRules, FName("LeftHandSocket"));
		
		AttachActorToSocket(EquippedMainHand, FName("LeftHandSocket"));
		// const USkeletalMeshSocket* LeftHandSocket = PlayerCharacter->GetMesh()->GetSocketByName(SocketName);
		// if (LeftHandSocket) {
		// 	LeftHandSocket->AttachActor(ActorToAttach, PlayerCharacter->GetMesh());
		// }
	}
}

void UInventoryComponent::AttachActorToSocket(UClass* ActorToAttach, const FName Socket) const
{
	if(!IsValid(PlayerCharacter) || PlayerCharacter->GetMesh() == nullptr || ActorToAttach == nullptr) return;
	
	// if(const USkeletalMeshSocket* SocketToAttach= PlayerCharacter->GetMesh()->GetSocketByName(Socket))
	// {
		// AActor* NewActor = ActorToAttach->GetDefaultObject<UItemBase>()->AssetData.Actor;
		// SocketToAttach->AttachActor(NewActor, PlayerCharacter->GetMesh());
	// }
}

/*
* Grid Inventory
*/
void UInventoryComponent::AddItemAt(UItemData_* ItemToAdd, int32 TopLeftIndex)
{
	const FGridTile Tile = InventoryIndexToTileXY(TopLeftIndex);
	const FIntPoint Dimensions = ItemToAdd->GetDimensions();
	
	for (int32 i = Tile.X; i < Tile.X + Dimensions.X - 1; i++)
	{
		for (int32 j = Tile.Y; j < Tile.Y + Dimensions.Y - 1; j++)
		{
			// Assign new item pointer to each tile it occupies, this is "recording" the item into inventory
			// [X - X - X ]
			// [X - X - X ]
			const FGridTile ValidTile(i, j);
			// Items.EmplaceAt(TileXYToInventoryIndex(ValidTile), ItemToAdd);
			Items[TileXYToInventoryIndex(ValidTile)] = ItemToAdd;
			GEngine->AddOnScreenDebugMessage(4, 3.f, FColor::Emerald, FString::Printf(TEXT("Items %d"), Items.Num()));
			OnInventoryUpdated.Broadcast();
		}
	}
}

bool UInventoryComponent::TryAddGridItem(UItemData_* ItemToAdd)
{
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Emerald, FString::Printf(TEXT("TryAddGridItem")));

	if(ItemToAdd)
	{
		GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Emerald, FString::Printf(TEXT("ItemToAdd is valid")));
		// For all Inventory indices, check if item can fit
		for (int32 i = 0; i < Items.Num(); i++)
		{
			if (IsRoomAvailable(ItemToAdd, i))
			// if (IsRoomAvailable(Items[i], i))
			{
				AddItemAt(ItemToAdd, i);
				return true;
			}
		}
	}
	return false;
}

bool UInventoryComponent::IsRoomAvailable(UItemData_* ItemToCheck, int32 TopLeftIndex)
{
	const FGridTile TileCoords = InventoryIndexToTileXY(TopLeftIndex);
	const FIntPoint Dimensions = ItemToCheck->GetDimensions();
	
	for (int32 i = TileCoords.X; i < TileCoords.X + Dimensions.X - 1; i++)
	{
		for (int32 j = TileCoords.Y; j < TileCoords.Y + Dimensions.Y - 1; j++)
		{
			if(IsTileValid(FGridTile(i, j)))
			{
				const FGridTile ValidTile(i, j);
				UItemData_* OutItem;
				if(GetItemAtIndex(TileXYToInventoryIndex(ValidTile), OutItem))
				{
					if(OutItem)
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}
	}
	return true;
}

FGridTile UInventoryComponent::InventoryIndexToTileXY(int32 Index)
{
	const int32 X = Index % GridColumns;
	const int32 Y = FMath::Floor( Index / GridColumns ) ;
		
	return FGridTile{ X, Y };
}

// Tile Coordinates start from 0, for 5 columns it's 0 to 4
int32 UInventoryComponent::TileXYToInventoryIndex(FGridTile TileCoords)
{
	return TileCoords.X + TileCoords.Y * GridColumns;
}

bool UInventoryComponent::IsTileValid(const FGridTile TileCoords)
{
	return TileCoords.X >= 0 && TileCoords.Y >= 0 && TileCoords.X < GridRows && TileCoords.Y < GridColumns;
	// return Tile.X >= 0 && Tile.Y >= 0 && Tile.X < 0 && Tile.Y < 0;
}

bool UInventoryComponent::GetItemAtIndex(int32 Index, UItemData_*& ItemFound)
{
	if(Items.IsValidIndex(Index))
	{
		ItemFound = Items[Index];
		return true;
	}
	ItemFound = nullptr;
	return false;
}


FGridTile UInventoryComponent::ForEachIndex(UItemData_* ItemToCheck, int32 TopLeftIndex)
{
	return  FGridTile();
}

















