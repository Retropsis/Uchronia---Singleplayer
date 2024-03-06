// Retropsis @ 2023-2024

#include "Vehicle/VehicleMovement.h"

UVehicleMovement::UVehicleMovement()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVehicleMovement::BeginPlay()
{
	Super::BeginPlay();	
}

void UVehicleMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

