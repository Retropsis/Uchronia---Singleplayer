// Retropsis @ 2023-2024


#include "Vehicle/FuelComponent.h"

UFuelComponent::UFuelComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFuelComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFuelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

