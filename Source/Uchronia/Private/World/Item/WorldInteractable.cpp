// Retropsis @ 2023-2024


#include "World/Item/WorldInteractable.h"
#include "ActorComponents/Inventory/InventoryComponent.h"
#include "Components/SphereComponent.h"

AWorldInteractable::AWorldInteractable()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	AActor::SetReplicateMovement(true);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractableMesh"));
	InteractableMesh->SetupAttachment(RootComponent);
	InteractableMesh->SetSimulatePhysics(bUsePhysics);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	InteractableMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// TODO: Change this in exiting overlap to close chest for example
	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(InteractableMesh);
	OverlapSphere->SetSphereRadius(OverlapRadius);
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AWorldInteractable::BeginPlay()
{
	Super::BeginPlay();
	InitializeInteractable(AWorldInteractable::StaticClass());
}

void AWorldInteractable::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if(ChangedPropertyName == GET_MEMBER_NAME_CHECKED(AWorldInteractable, DesiredItemID))
	{
		if(ItemDataTable)
		{
			if(const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString()))
			{
				InteractableMesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}
		}
	}
}

void AWorldInteractable::InitializeInteractable(const TSubclassOf<AWorldInteractable> BaseClass)
{
	if(IsValid(ItemDataTable) && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		InteractableReference = NewObject<AWorldInteractable>(this, BaseClass);
		InteractableReference->ID = ItemData->ID;
		InteractableReference->TextData = ItemData->TextData;
		InteractableReference->AssetData = ItemData->AssetData;
		InteractableMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}

void AWorldInteractable::SetQuantity(int32 NewQuantity)
{
	if(NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize : 1);

		// if(OverlappingInventory)
		// {
		// 	if(Quantity <= 0)
		// 	{
		// 		OverlappingInventory->RemoveSingleInstanceOfItem(this);
		// 	}
		// }
	}
}

/*
 * Interaction Interface
 */
void AWorldInteractable::BeginFocus()
{
	// Highlight and shit
}

void AWorldInteractable::EndFocus()
{
	// End highlight and shit
}

void AWorldInteractable::Interact(APlayerCharacter* PlayerCharacter)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Emerald, FString::Printf(TEXT("Interacting")));
	switch (InstanceInteractableData.InteractableType)
	{
	case EInteractableType::EIT_Pickup:
		TryPickup(PlayerCharacter);;
		break;
	case EInteractableType::EIT_NonPlayableCharacter:
		break;
	case EInteractableType::EIT_Device:
		break;
	case EInteractableType::EIT_Toggle:
		break;
	case EInteractableType::EIT_Container:
		TryOpenContainer(PlayerCharacter);
		break;
	default: ;
	}
}

void AWorldInteractable::TryPickup_Implementation(APlayerCharacter* PlayerCharacter)
{
}

void AWorldInteractable::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = /*EInteractableType::EIT_Container*/InteractableType;
	InstanceInteractableData.Action = InteractableReference->TextData.Interaction;
	InstanceInteractableData.Name = InteractableReference->TextData.Name;
	InteractableData = InstanceInteractableData;
}
