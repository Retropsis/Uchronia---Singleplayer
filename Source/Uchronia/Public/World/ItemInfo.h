#pragma once

#include "CoreMinimal.h"
#include  "ItemInfo.generated.h"

class AItem;

USTRUCT(BlueprintType)
struct FItemStruct
{
	GENERATED_BODY();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AItem> ItemClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> ItemMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName ItemName = FName();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UTexture2D> ItemIcon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ItemValue;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ItemWeight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemQuantity = 1;
};
