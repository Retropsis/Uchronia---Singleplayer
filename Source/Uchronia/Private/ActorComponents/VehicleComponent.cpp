// Retropsis @ 2023-2024

#include "ActorComponents/VehicleComponent.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Player/CharacterPlayerController.h"
#include "Vehicle/Vehicle.h"

UVehicleComponent::UVehicleComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UVehicleComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UVehicleComponent, bIsInVehicle);
}

void UVehicleComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningCharacter = Cast<APlayerCharacter>(GetOwner());
}

void UVehicleComponent::InitializeVehicle()
{
	// HUD Init
}

void UVehicleComponent::InteractWithVehicle()
{
	if(bIsInVehicle)
	{
		// This is only for Passengers because they are not possessing the vehicle and still have a vehicle component
		ServerExitVehicle();
		ExitVehicle();
	}
	else
	{
		FVector Start;
		FRotator StartRotation;
		OwningCharacter->GetController()->GetPlayerViewPoint(Start, StartRotation);
		FVector End = Start + OwningCharacter->GetController()->GetControlRotation().Vector() * InteractionDistance;
		TArray<AActor*> ActorsToIgnore;
		FHitResult HitResult;
		if(UKismetSystemLibrary::LineTraceSingle(this, Start, End, TraceTypeQuery1, false,
			ActorsToIgnore, EDrawDebugTrace::None, HitResult, true))
		{
			// if (IVehicleInterface* VehicleInterface = Cast<IVehicleInterface>(HitResult.GetActor()))
			if (HitResult.GetActor()->Implements<UVehicleInterface>() && HitResult.GetComponent()->ComponentHasTag(FName("Controller")))
			{
				// if(VehicleInterface && HitResult.GetComponent()->ComponentHasTag(FName("Controller")))
				// {
					// InteractingVehicle = VehicleInterface->Execute_GetOwningVehicle(this);
					IVehicleInterface* VehicleInterface = Cast<IVehicleInterface>(HitResult.GetActor());
					InteractingVehicle = VehicleInterface->Execute_GetOwningVehicle(HitResult.GetActor());
					// InteractingVehicle = IVehicleInterface::Execute_GetOwningVehicle(HitResult.GetActor());
					// InteractingVehicle = HitResult.GetActor();
					ServerTryEnterVehicle(InteractingVehicle);
				// }
			}
		}
	}
}

void UVehicleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UVehicleComponent::TryEnterVehicle(AVehicle* VehicleToEnter)
{
	if(const IVehicleInterface* VehicleInterface = Cast<IVehicleInterface>(VehicleToEnter))
	{
		if(VehicleInterface->Execute_IsAnySeatAvailable(VehicleToEnter))
		{
			if (ACharacterPlayerController* PlayerController = Cast<ACharacterPlayerController>(OwningCharacter->GetController()))
			{
				PlayerController->TryAddingMappingContext(VehicleContext);
				bIsInVehicle = true;
				
				OwningCharacter->ToggleCollisions(false);
				OwningCharacter->ClientToggleCollisions(false);
				OwningCharacter->MulticastToggleCollisions(false);
				
				switch (ESeatType Seat = VehicleInterface->Execute_GetFirstAvailableSeat(VehicleToEnter)) {
				case ESeatType::EST_None:
					break;
				case ESeatType::EST_Driver:
					PlayerController->EnableCharacterContext(false);
					PlayerController->Possess(InteractingVehicle);
					InteractingVehicle->OccupySeat(OwningCharacter, ESeatType::EST_Driver, true);
					MulticastAttachPlayerToSeat(OwningCharacter, InteractingVehicle, "DriverSeat");
					OccupyingSeat = ESeatType::EST_Driver;
					break;
				case ESeatType::EST_Passenger_1:
					InteractingVehicle->OccupySeat(OwningCharacter, ESeatType::EST_Passenger_1, true);
					MulticastAttachPlayerToSeat(OwningCharacter, InteractingVehicle, "PassengerSeat_01");
					OccupyingSeat = ESeatType::EST_Passenger_1;
					break;
				case ESeatType::EST_Passenger_2:
					break;
				case ESeatType::EST_Passenger_3:
					break;
				case ESeatType::EST_Passenger_4:
					break;
				case ESeatType::EST_MAX:
					break;
				default: ;
				}
			}
		}
	}
}

void UVehicleComponent::ServerTryEnterVehicle_Implementation(AVehicle* VehicleToEnter)
{
	TryEnterVehicle(VehicleToEnter);
}

void UVehicleComponent::ServerExitVehicle_Implementation()
{
	bIsInVehicle = false;
	OwningCharacter->ToggleCollisions(true);
	OwningCharacter->ClientToggleCollisions(true);
	OwningCharacter->MulticastToggleCollisions(true);
	MulticastDetachPlayerFromSeat();
	
	InteractingVehicle->OccupySeat(nullptr, OccupyingSeat, false);
	OccupyingSeat = ESeatType::EST_None;
	
	UKismetSystemLibrary::MoveComponentTo(
		OwningCharacter->GetRootComponent(),
		OwningCharacter->GetActorLocation(),
		FRotator(),
		false, false, 0.2f,
		false,
		EMoveComponentAction::Move,
		FLatentActionInfo());
	
		InteractingVehicle = nullptr;
}

void UVehicleComponent::ExitVehicle()
{
	if (ACharacterPlayerController* PlayerController = Cast<ACharacterPlayerController>(OwningCharacter->GetController()))
	{
		PlayerController->EnableCharacterContext(true);
		PlayerController->RemoveMappingContext(VehicleContext);
		PlayerController->RemoveMappingContext(InteractingVehicle->GetVehicleControlsContext());
	}
}

void UVehicleComponent::MulticastAttachPlayerToSeat_Implementation(APlayerCharacter* PlayerToAttach, AVehicle* InVehicle, FName SocketName)
{
	const FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	PlayerToAttach->AttachToActor(InVehicle, TransformRules, SocketName);
}

void UVehicleComponent::MulticastDetachPlayerFromSeat_Implementation()
{
	const FDetachmentTransformRules TransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	OwningCharacter->DetachFromActor(TransformRules);
}
