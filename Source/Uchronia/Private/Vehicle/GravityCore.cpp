// Retropsis @ 2023-2024


#include "Vehicle/GravityCore.h"

UGravityCore::UGravityCore()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGravityCore::BeginPlay()
{
	Super::BeginPlay();
}

void UGravityCore::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

