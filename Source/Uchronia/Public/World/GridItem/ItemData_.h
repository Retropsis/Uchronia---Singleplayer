// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemData_.generated.h"

class AItem_;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UCHRONIA_API UItemData_ : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	FIntPoint Dimensions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	TObjectPtr<UTexture2D> ItemIcon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	TObjectPtr<UTexture2D> ItemIconRotated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	TObjectPtr<UMaterialInterface> ItemIconMat;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	TObjectPtr<UMaterialInterface> ItemIconRotatedMat;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	TSubclassOf<AItem_> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	bool bRotated = false;
	
	FORCEINLINE FIntPoint GetDimensions() const { return Dimensions; }
	FORCEINLINE int32 Height() const { return Dimensions.X; }
	FORCEINLINE int32 Width() const { return Dimensions.Y; }
};
