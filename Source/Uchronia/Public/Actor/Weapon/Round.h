// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Actor/Weapon/Projectile.h"
#include "Round.generated.h"

/**
 * 
 */
UCLASS()
class UCHRONIA_API ARound : public AProjectile
{
	GENERATED_BODY()

public:
	ARound();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

private:
	/*
	 * SFX / VFX TODO: Should be DataAsset
	 */
	UPROPERTY(EditAnywhere, Category="Weapon Properties|SFX")
	TObjectPtr<USoundBase> BounceSound;
};
