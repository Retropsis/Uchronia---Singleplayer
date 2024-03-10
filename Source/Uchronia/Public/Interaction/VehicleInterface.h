// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Vehicle/VehicleData.h"
#include "VehicleInterface.generated.h"

class IFuelInterface;
class AVehicle;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVehicleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UCHRONIA_API IVehicleInterface
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsAnySeatAvailable();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ESeatType GetFirstAvailableSeat();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AVehicle* GetOwningVehicle();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EDrivingPose GetDrivingPose();
};
