// Retropsis @ 2023-2024


#include "Vehicle/SKM_ComponentCore.h"

#include "Vehicle/HardPointTraceComponent.h"
#include "Vehicle/Vehicle.h"

void USKM_ComponentCore::BeginPlay()
{
	Super::BeginPlay();
}

void USKM_ComponentCore::InitializeComponentCore(UHardPointTraceComponent* HardPointTrace)
{
	HardPointTraceComponent = HardPointTrace;
	HardPointTraceComponent->HardPointComponent = this;
}

AVehicle* USKM_ComponentCore::GetOwningVehicle_Implementation()
{
	return IComponentInterface::GetOwningVehicle_Implementation();
}
