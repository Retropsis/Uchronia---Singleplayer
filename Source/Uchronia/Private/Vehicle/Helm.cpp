// Retropsis @ 2023-2024


#include "Vehicle/Helm.h"

#include "Inventory/Data/ItemData.h"
#include "Net/UnrealNetwork.h"
#include "Vehicle/Vehicle.h"

AHelm::AHelm()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AHelm::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AHelm::BeginPlay()
{
	Super::BeginPlay();
	UpdateInteractableData();
}

void AHelm::SetVehicleReference(AVehicle* VehicleRef)
{
	if(VehicleRef)
	{
		VehicleReference = VehicleRef;
	}
}

void AHelm::BeginFocus()
{
}

void AHelm::EndFocus()
{
}

void AHelm::BeginInteract()
{
}

void AHelm::EndInteract()
{
}

void AHelm::UpdateInteractableData()
{
	if(IsValid(ItemDataTable) && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());
		InstanceInteractableData.Action = ItemData->TextData.Interaction;
		InstanceInteractableData.Name = ItemData->TextData.Name;
		InstanceInteractableData.InteractableType = EInteractableType::EIT_Device;
		InteractableData = InstanceInteractableData;
	}
}

void AHelm::Interact(APlayerCharacter* PlayerCharacter)
{
}
