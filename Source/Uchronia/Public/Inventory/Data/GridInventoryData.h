#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GridInventoryData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FGridLine
{
	GENERATED_BODY()
	
	FGridLine() {};
	FGridLine(FVector2D InStart, FVector2D InEnd) : Start(InStart), End(InEnd)
	{
		
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Start = FVector2D::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D End = FVector2D::ZeroVector;
};

USTRUCT(Blueprintable, BlueprintType)
struct FGridTile
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y = 0;
};
