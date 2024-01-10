// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Actor/Weapon/HitScanWeapon.h"
#include "MultiHitScanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UCHRONIA_API AMultiHitScanWeapon : public AHitScanWeapon
{
	GENERATED_BODY()

public:
	virtual void Trigger(const FVector& HitTarget) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties")
	uint32 NumHits = 10;
};
