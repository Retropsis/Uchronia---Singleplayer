// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Types/AnimationStates.h"
#include "CharacterAnimInstance.generated.h"

class ARangeWeapon;
class AWeapon;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual  void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayFireMontage(bool bIsAiming);
	void PlayReloadMontage();
	void PlayHitReactMontage(const FVector& ImpactPoint);
	void JumpToReloadEnd();
	void DirectionalHitReact(const FVector& ImpactPoint);

	UFUNCTION(BlueprintCallable)
	EPhysicalSurface GetSurfaceType();

protected:
	UPROPERTY(BlueprintReadOnly, Category="CharacterMovement")
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category="CharacterMovement")
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category="CharacterMovement")
	bool bAirborne = false;

	UPROPERTY(BlueprintReadOnly, Category="CharacterMovement")
	bool bIsAccelerating = false;

	UPROPERTY(BlueprintReadOnly, Category="CharacterMovement")
	bool bWeaponEquipped = false;

	TObjectPtr<ARangeWeapon> EquippedWeapon;

	UPROPERTY(BlueprintReadOnly, Category="CharacterMovement")
	bool bIsCrouched = false;

	UPROPERTY(BlueprintReadOnly, Category="CharacterMovement")
	bool bAiming = false;

	UPROPERTY(BlueprintReadOnly, Category="CharacterMovement")
	float YawOffset;

	UPROPERTY(EditDefaultsOnly, Category="CharacterMovement")
	float YawOffsetInterpSpeed = 6.f;
	
	UPROPERTY(EditDefaultsOnly, Category="CharacterMovement")
	float LeanInterpSpeed = 6.f;
	
	UPROPERTY(BlueprintReadOnly, Category="CharacterMovement")
	float Lean;
	
	UPROPERTY(BlueprintReadOnly, Category="CharacterMovement")
	float AO_Yaw;
	
	UPROPERTY(BlueprintReadOnly, Category="CharacterMovement")
	float AO_Pitch;

	UPROPERTY(BlueprintReadOnly, Category="CharacterMovement")
	FTransform LeftHandTransform;
	
	UPROPERTY(BlueprintReadOnly, Category="CharacterMovement")
	FRotator RightHandRotation;
	
	UPROPERTY(BlueprintReadOnly, Category="CharacterMovement")
	ETurningInPlace TurningInPlace;

	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;

	UPROPERTY(EditDefaultsOnly, Category="Montage")
	TObjectPtr<UAnimMontage> FireWeaponMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Montage")
	TObjectPtr<UAnimMontage> ReloadMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Montage")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(BlueprintReadOnly)
	bool bIsLocallyControlled = false;

	UPROPERTY(BlueprintReadOnly, Category="CharacterMovement")
	bool bRotateRootBone;
	
	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bUseFABRIK;
	
	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bUseAimOffsets;
	
	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bTransformRightHand;
	
	UPROPERTY(BlueprintReadOnly, Category="CharacterMovement")
	bool bSwimming;
};
