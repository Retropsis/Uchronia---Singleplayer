#pragma once

#include "CoreMinimal.h"
#include "InventoryData.generated.h"

USTRUCT(BlueprintType)
struct FTile
{
	GENERATED_BODY()

	int32 X = 0;
	int32 Y = 0;
};

USTRUCT(BlueprintType)
struct FLine
{
	GENERATED_BODY()

	FVector2D Start = FVector2D::ZeroVector;
	FVector2D End = FVector2D::ZeroVector;
};

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	EIT_None UMETA(DisplayName="None"),
	EIT_Inventory UMETA(DisplayName="Inventory"),
	EIT_MainHand UMETA(DisplayName="MainHand"),
	EIT_Offhand UMETA(DisplayName="Offhand"),
	EIT_AnyHand UMETA(DisplayName="AnyHand"),
	EIT_Head UMETA(DisplayName="Head"),
	EIT_UpperBody UMETA(DisplayName="UpperBody"),
	EIT_LowerBody UMETA(DisplayName="LowerBody"),
	EIT_Belt UMETA(DisplayName="Belt"),
	EIT_Feet UMETA(DisplayName="Feet"),
	EIT_Armor UMETA(DisplayName="Armor"),
	EIT_Backpack UMETA(DisplayName="Backpack"),
	EIT_Vicinity UMETA(DisplayName="Vicinity"),
};