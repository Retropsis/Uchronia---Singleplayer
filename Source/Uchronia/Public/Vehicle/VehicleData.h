#pragma once

#include "CoreMinimal.h"
#include "VehicleData.generated.h"

UENUM(BlueprintType)
enum class ESeatType : uint8
{
	EST_None UMETA(DisplayName="None"),
	EST_Driver UMETA(DisplayName="Driver"),
	EST_Passenger_1 UMETA(DisplayName="Passenger_1"),
	EST_Passenger_2 UMETA(DisplayName="Passenger_2"),
	EST_Passenger_3 UMETA(DisplayName="Passenger_3"),
	EST_Passenger_4 UMETA(DisplayName="Passenger_4"),
	EST_MAX UMETA(DisplayName="DefaultMAX")
};

UENUM(BlueprintType)
enum class EGears : uint8
{
	EST_R UMETA(DisplayName="Reverse"),
	EST_N UMETA(DisplayName="Neutral"),
	EST_1 UMETA(DisplayName="1"),
	EST_2 UMETA(DisplayName="2"),
	EST_3 UMETA(DisplayName="3"),
	EST_4 UMETA(DisplayName="4"),
	EST_5 UMETA(DisplayName="5"),
	EST_6 UMETA(DisplayName="6"),
	EG_MAX UMETA(DisplayName="DefaultMAX")
};

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	EMS_None UMETA(DisplayName="None"),
	EMS_Engine_Off UMETA(DisplayName="Engine_Off"),
	EMS_Idle UMETA(DisplayName="Idle"),
	EMS_Moving UMETA(DisplayName="Moving"),
	EMS_Ease_In UMETA(DisplayName="Ease_In"),
	EMS_Ease_Out UMETA(DisplayName="Ease_Out"),
	EMS_MAX UMETA(DisplayName="DefaultMAX")
};