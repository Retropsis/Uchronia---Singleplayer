#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EHardPoint : uint8
{
	EHP_None UMETA(DisplayName="None"),
	EHP_Hatch_Front_L UMETA(DisplayName="Hatch_Front_L"),
	EHP_Hatch_Front_R UMETA(DisplayName="Hatch_Front_R"),
	EHP_Hatch_Middle_L UMETA(DisplayName="Hatch_Middle_L"),
	EHP_Hatch_Middle_R UMETA(DisplayName="Hatch_Middle_R"),
	EHP_Hatch_Back_L UMETA(DisplayName="Hatch_Back_L"),
	EHP_Hatch_Back_R UMETA(DisplayName="Hatch_Back_R"),
	EHP_Helm UMETA(DisplayName="Helm"),
	EHP_Engine_L UMETA(DisplayName="Engine_L"),
	EHP_Engine_R UMETA(DisplayName="Engine_R"),
	EHP_Lampost UMETA(DisplayName="Lampost"),
	
	EHP_MAX UMETA(DisplayName="DefaultMAX")
};
