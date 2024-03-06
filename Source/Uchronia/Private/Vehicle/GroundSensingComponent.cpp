// Retropsis @ 2023-2024

#include "Vehicle/GroundSensingComponent.h"

UGroundSensingComponent::UGroundSensingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGroundSensingComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UGroundSensingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

