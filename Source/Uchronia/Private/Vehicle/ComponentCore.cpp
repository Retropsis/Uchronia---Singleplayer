// Retropsis @ 2023-2024


#include "Vehicle/ComponentCore.h"
#include "Vehicle/Vehicle.h"

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

void AComponentCore::InitializeComponentCore(AVehicle* InVehicle)
{
	OwningVehicle = InVehicle;
	OwningVehicle->OnGearChangeDelegate.AddDynamic(this, &AComponentCore::HandleOnGearChanged);
	SetAnimInstance();
}

void AComponentCore::HandleOnGearChanged(EGears NewGear)
{
	switch (NewGear) {
	case EGears::EST_N:
		ToggleEngineTrail(false);
		break;
	case EGears::EST_R:
	case EGears::EST_1:
	case EGears::EST_2:
	case EGears::EST_3:
	case EGears::EST_4:
	case EGears::EST_5:
	case EGears::EST_6:
		ToggleEngineTrail(true);
		break;
	case EGears::EG_MAX:
		break;
	default: ;
	}
}

