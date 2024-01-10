// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Actor/Weapon/WeaponTypes.h"
#include "Item/Pickup.h"
#include "Ammunition.generated.h"

/**
 * 
 */
UCLASS()
class UCHRONIA_API AAmmunition : public APickup
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
private:
	UPROPERTY(EditAnywhere, Category="Pickup Properties|Ammunition")
	EWeaponType WeaponType = EWeaponType::EWT_9mm;
	
	UPROPERTY(EditAnywhere, Category="Pickup Properties|Ammunition")
	int32 AmmoAmount = 0;

	UPROPERTY(EditAnywhere, Category="Pickup Properties|Ammunition")
	int32 MinAmount = 1;
	
	UPROPERTY(EditAnywhere, Category="Pickup Properties|Ammunition")
	int32 MaxAmount = 8;
};
