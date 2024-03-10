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
enum class EDrivingPose : uint8
{
	EDP_None UMETA(DisplayName="None"),
	EDP_Moped_Driver UMETA(DisplayName="Moped_Driver"),
	EDP_Skimmer_Driver UMETA(DisplayName="Skimmer_Driver"),
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
	EMS_Engine_Off_Unable UMETA(DisplayName="Engine_Off_Unable"),
	EMS_Engine_Off_Ready UMETA(DisplayName="Engine_Off_Ready"),
	EMS_Engine_On_Idling UMETA(DisplayName="Engine_On_Idling"),
	EMS_Engine_On_Moving UMETA(DisplayName="Engine_On_Moving"),
	EMS_Engine_On_Easing_In UMETA(DisplayName="Engine_On_Easing_In"),
	EMS_Engine_On_Easing_Out UMETA(DisplayName="Engine_On_Easing_Out"),
	EMS_MAX UMETA(DisplayName="DefaultMAX")
};