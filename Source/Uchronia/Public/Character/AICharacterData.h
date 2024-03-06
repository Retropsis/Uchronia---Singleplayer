#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, Blueprintable)
enum class EEnemyStates : uint8
{
	EES_None UMETA(DisplayName="None"),
	EES_Idle UMETA(DisplayName="Idle"),
	EES_Patrol UMETA(DisplayName="Patrol"),
	EES_Chase UMETA(DisplayName="Chase"),
	EES_Fall UMETA(DisplayName="Fall"),
	EES_Investigate UMETA(DisplayName="Investigate"),
	
	EES_MAX UMETA(DisplayName="DefaultMAX")
};

UENUM(BlueprintType, Blueprintable)
enum class EChasingStates : uint8
{
	ECS_None UMETA(DisplayName="None"),
	ECS_Chase UMETA(DisplayName="Chase"),
	ECS_HitReact UMETA(DisplayName="HitReact"),
	ECS_Dead UMETA(DisplayName="Dead"),
	
	ECS_MAX UMETA(DisplayName="DefaultMAX")
};