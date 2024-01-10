// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AIAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UCHRONIA_API UAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:
	void PlayHitReactMontage(const FVector& ImpactPoint);

protected:
	UPROPERTY(EditDefaultsOnly, Category="Montage")
	TObjectPtr<UAnimMontage> HitReactMontage;

public:
	UAnimMontage* GetHitReactMontage() const { return HitReactMontage; }
};
