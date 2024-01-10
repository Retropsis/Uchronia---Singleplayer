// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "World/Item.h"
#include "Currency.generated.h"

/**
 * 
 */
UCLASS()
class UCHRONIA_API ACurrency : public AItem
{
	GENERATED_BODY()

public:
	ACurrency();

	UPROPERTY(EditDefaultsOnly, Category="Currency")
	FName CurrencyName = FName("Fuel Unit");
	
	virtual  void Use(APlayerCharacter* PlayerCharacter) override;
	
};
