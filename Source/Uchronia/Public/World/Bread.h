// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "World/Item.h"
#include "Bread.generated.h"

/**
 * 
 */
UCLASS()
class UCHRONIA_API ABread : public AItem
{
	GENERATED_BODY()
	ABread();
	
	virtual void Use(APlayerCharacter* PlayerCharacter) override;

protected:
	UPROPERTY(EditAnywhere)
	float HungerToRecover = 10.f;
};
