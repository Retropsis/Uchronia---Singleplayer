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
	
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button Mesh"));
	ButtonMesh->SetupAttachment(RootComponent);
	ButtonMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ButtonMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
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
	if(PlayerCharacter)
	{
		InitiateMovement();
	}
}

void AHelm::InitiateMovement()
{
	if(VehicleReference)
	{
		switch (HelmDirection)
		{
		case EHelmDirection::EHD_Stop:
			VehicleReference->SetDirection(FVector2D{0.f, 0.f});
			VehicleReference->bIsMoving = false;
			break;
		case EHelmDirection::EHD_Forward:
			VehicleReference->SetDirection(FVector2D{1.f, 0.f});
			VehicleReference->bIsMoving = true;
			break;
		case EHelmDirection::EHD_Left:
			VehicleReference->SetDirection(FVector2D{0.f, -1.f});
			VehicleReference->bIsMoving = true;
			break;
		case EHelmDirection::EHD_Right:
			VehicleReference->SetDirection(FVector2D{0.f, 1.f});
			VehicleReference->bIsMoving = true;
			break;
		case EHelmDirection::EHD_Reverse:
			VehicleReference->SetDirection(FVector2D{-1.f, 0.f});
			VehicleReference->bIsMoving = true;
			break;
		default: ;
		}
	}
}
