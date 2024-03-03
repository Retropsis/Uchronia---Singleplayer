// Retropsis @ 2023-2024

#include "Vehicle/SkimmerMovement.h"

#include "Kismet/KismetStringLibrary.h"
#include "Vehicle/VehicleCore.h"

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
	ToggleWaterSplashes();
}

void USkimmerMovement::InitializeSkimmerMovement(UVehicleCore* InVehicleCore)
{
	VehicleCore = InVehicleCore;
}

void USkimmerMovement::ToggleWaterSplashes()
{
	if(GetVehicleElevation() < WaterSplashThreshold && !bEnableWaterSplash)
	{
		if(GetWorld()) GetWorld()->GetTimerManager().SetTimer(WaterSplashTimer, this, &USkimmerMovement::CallWaterSplash, WaterSplashCooldown, true);
		bEnableWaterSplash = true;
	}
	if(GetVehicleElevation() > WaterSplashThreshold)
	{
		if(GetWorld()) GetWorld()->GetTimerManager().ClearTimer(WaterSplashTimer);
		bEnableWaterSplash = false;
	}
}

void USkimmerMovement::CallWaterSplash()
{
	if(IsValid(VehicleCore))
	{
		VehicleCore->WaterSplashAtLocation();
	}
}

float USkimmerMovement::GetVehicleElevation()
{
	return OwningVehicle->GetActorLocation().Z;
}

