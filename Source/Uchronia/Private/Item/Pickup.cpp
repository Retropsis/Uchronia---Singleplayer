// Retropsis @ 2023-2024

#include "Item/Pickup.h"
#include "ActorComponents/Inventory/InventoryComponent.h"
#include "ActorComponents/Inventory/ItemBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(RootComponent);
	PickupMesh->SetSimulatePhysics(true);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PickupMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// TODO: Will have to remove this after pickup functionality is done by tracing instead
	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(PickupMesh);
	OverlapSphere->SetSphereRadius(OverlapRadius);
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	// OverlapSphere->AddLocalOffset(FVector(0.f, 0.f, 80.f)); 
}

// void APickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
// {
// 	Super::PostEditChangeProperty(PropertyChangedEvent);
//
// 	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
// 	if(ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickup, DesiredItemID))
// 	{
// 		if(ItemDataTable)
// 		{
// 			if(const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString()))
// 			{
// 				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
// 			}
// 		}
// 	}
// }

void APickup::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnBeginOverlap);
	}
	// TODO: Investigate if it needs to be authoritative then passed down to clients
	InitializePickup(UItemBase::StaticClass(), ItemQuantity);
}

void APickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

/*
 * Interaction
 */
void APickup::BeginFocus()
{
	if(PickupMesh)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, FString::Printf(TEXT("BeginFocus")));
		// Mesh->SetRenderCustomDepth(true);
	}
}

void APickup::EndFocus()
{
	if(PickupMesh)
	{
		GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Red, FString::Printf(TEXT("EndFocus")));
		// Mesh->SetRenderCustomDepth(false);
	}
}

//~ Server Only
void APickup::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	if(IsValid(ItemDataTable) && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		ItemReference = NewObject<UItemBase>(this, BaseClass);
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

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}
//~ Server Only

void APickup::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight();
	ItemReference->OwningInventory = nullptr;
	PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);
	UpdateInteractableData();
}

void APickup::Interact(APlayerCharacter* PlayerCharacter)
{
	// TODO: might need to be authoritative
	if(PlayerCharacter)
	{
		TakePickup(PlayerCharacter);
	}
}

void APickup::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::EIT_Pickup;
	InstanceInteractableData.Action = ItemReference->TextData.Interaction;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData;
}

void APickup::TakePickup(const APlayerCharacter* Taker)
{
	if (IsValid(this))
	{
		if(ItemReference)
		{
			if(UInventoryComponent* PlayerInventory = Taker->GetInventory())
			{
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);
				
				switch (AddResult.OperationResult)
				{
				case EItemAddResult::IAR_NoItemAdded:
					break;
				case EItemAddResult::IAR_PartialAmountItemAdded:
					UpdateInteractableData();
					Taker->UpdateInteractionWidget();
					break;
				case EItemAddResult::IAR_AllItemAdded:
					 Taker->GetInventory()->ServerDestroyPickup(this);
					break;
				default: ;
				}
				UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Player Inventory component is null."));				
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Item Reference is null."));				
		}
	}
}

void APickup::Destroyed()
{
	Super::Destroyed();
	if(IsValid(PickupSound)) UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
}

