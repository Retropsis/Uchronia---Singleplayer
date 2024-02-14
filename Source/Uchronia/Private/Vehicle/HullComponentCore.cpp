// Retropsis @ 2023-2024


#include "Vehicle/HullComponentCore.h"

#include "Vehicle/HardPointTraceComponent.h"

UHullComponentCore::UHullComponentCore()
{
	HardPointTrace = CreateDefaultSubobject<UHardPointTraceComponent>(TEXT("HardPointTrace"));
	HardPointTrace->SetupAttachment(this);
	HardPointTrace->HardPointComponent = this;

	SmokeSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SmokeSpawnLocation"));
	SmokeSpawnLocation->SetupAttachment(this);
}
