// Retropsis @ 2023-2024

#include "Vehicle/SkimmerMovement.h"

#include "Vehicle/Vehicle.h"

USkimmerMovement::USkimmerMovement()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USkimmerMovement::BeginPlay()
{
	OwningVehicle = Cast<APawn>(GetOwner());
	HullReference = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	Super::BeginPlay();
}

void USkimmerMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

