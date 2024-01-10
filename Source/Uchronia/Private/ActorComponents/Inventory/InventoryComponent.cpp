// Retropsis @ 2023-2024

#include "ActorComponents/Inventory/InventoryComponent.h"
#include "ActorComponents/Inventory/ItemBase.h"
#include "Item/Pickup.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "World/WorldItem_.h"

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
	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if(PlayerCharacter)
	{
		PlayerCharacter->SetInventoryComponent(this);
	}
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
	switch (SlotType) {
	case EItemType::EIT_Weapon:
		// ClientSetEquippedMainHand(ItemToEquip->AssetData.Actor->GetClass());
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
* T4
*/
void UInventoryComponent::AddItemToInventory(AWorldItem_* ItemToAdd, int32 AmountToAdd)
{
	// TODO: Need to investigate client dupe and eaten drop, but both could be overlap related, need to nuke it
	if(ItemToAdd == nullptr) return;
	
	Inventory_.Add(ItemToAdd->GetClass());
	// InventoryWidget->UpdateInventory(Inventory_);
	ItemToAdd->OwningInventory = this;
	if(ItemToAdd->InteractableSound)
	{
		if(ItemToAdd->bSoundShouldPropagate)
		{
			if (PlayerCharacter->HasAuthority())
            {
            	MulticastPlayerSound(ItemToAdd->InteractableSound);
            }
            else
            {
            	ServerPlayerSound(ItemToAdd->InteractableSound);
            }
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(this, ItemToAdd->InteractableSound, PlayerCharacter->GetActorLocation());
		}
	}
}

void UInventoryComponent::ServerPlayerSound_Implementation(USoundBase* InSound)
{
	MulticastPlayerSound(InSound);	
}

void UInventoryComponent::MulticastPlayerSound_Implementation(USoundBase* InSound)
{
	if (InSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, InSound, PlayerCharacter->GetActorLocation());
	}
}

// Used
void UInventoryComponent::ServerSpawnIem_Implementation(TSubclassOf<AWorldItem_> ItemToSpawn, FTransform SpawnTransform)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerCharacter;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector Location;
	FVector Direction;
	UGameplayStatics::GetPlayerController(this, 0)->DeprojectMousePositionToWorld(Location, Direction);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult TraceHit;
	UKismetSystemLibrary::LineTraceSingle(this, Location, Location + Direction * 150.f, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, TraceHit, true, FLinearColor::White);

	FVector RandomLocation{ FMath::FRandRange(-75.f, 75.f), FMath::FRandRange(-75.f, 75.f), 75.f };
	const FVector SpawnLocation{ PlayerCharacter->GetActorLocation() + (PlayerCharacter->GetActorForwardVector() * ItemSpawnDistance + RandomLocation) };
	const FTransform SpawnTransform_{ PlayerCharacter->GetActorRotation(), SpawnLocation };
	GetWorld()->SpawnActor<AWorldItem_>(ItemToSpawn, SpawnTransform_, SpawnParams);
}

// Skipped
void UInventoryComponent::ClientSpawnIem_Implementation(TSubclassOf<AWorldItem_> ItemToSpawn, FTransform SpawnTransform)
{
	ServerSpawnIem(ItemToSpawn, SpawnTransform);
}

void UInventoryComponent::DropItemFromInventory(TSubclassOf<AWorldItem_> ItemToDrop)
{
	// ClientSpawnIem(ItemToDrop, FTransform());
	ServerSpawnIem(ItemToDrop, FTransform());
	Inventory_.RemoveAt(Inventory_.Find(ItemToDrop));
}

void UInventoryComponent::ServerDestroyActor_Implementation(AActor* ActorToDestroy)
{
	ActorToDestroy->Destroy();
}
