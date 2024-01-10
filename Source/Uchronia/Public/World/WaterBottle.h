// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "World/Item.h"
#include "WaterBottle.generated.h"

/**
 * 
 */
UCLASS()
class UCHRONIA_API AWaterBottle : public AItem
{
	GENERATED_BODY()
	AWaterBottle();
	virtual void Use(APlayerCharacter* PlayerCharacter) override;

protected:
	UPROPERTY(EditAnywhere)
	float HealthToRecover = 10.f;
};
