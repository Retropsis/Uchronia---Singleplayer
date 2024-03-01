// Retropsis @ 2023-2024

#include "Vehicle/VehicleCore.h"
#include "Vehicle/Vehicle.h"

UVehicleCore::UVehicleCore()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UVehicleCore::BeginPlay()
{
	Super::BeginPlay();
	OwningVehicle = Cast<AVehicle>(GetOwner());
}

void UVehicleCore::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

