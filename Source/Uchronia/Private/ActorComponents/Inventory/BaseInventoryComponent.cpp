// Retropsis @ 2023-2024


#include "ActorComponents/Inventory/BaseInventoryComponent.h"

UBaseInventoryComponent::UBaseInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBaseInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBaseInventoryComponent::TryInteract()
{
	if (OverlappingItems.Num() > 0)
	{
		// InteractingItem
	}
}
