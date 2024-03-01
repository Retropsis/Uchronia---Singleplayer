// Retropsis @ 2023-2024


#include "Vehicle/ComponentCore.h"

AComponentCore::AComponentCore()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AComponentCore::BeginPlay()
{
	Super::BeginPlay();
}

void AComponentCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

