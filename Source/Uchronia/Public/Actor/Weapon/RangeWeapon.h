// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Actor/Weapon/Weapon.h"
#include "RangeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UCHRONIA_API ARangeWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	ARangeWeapon();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;
	virtual void Trigger(const FVector& HitTarget);
	void AddRounds(int32 RoundsToAdd);
	void SetHUDAmmo();

	/*
	 * DATA
	 */
	/* Automatic Fire */
	float FireInterval = .15f;
	bool bAutomatic = false;
	bool bVisualizeScatter = false;
	float MarksmanFOV = 30.f;
	float MarksmanInterpSpeed = 20.f;

	/* Aiming Modifiers */
	float RecoveryDelay = 1.f;
	float RecoveryPerSecond = 1.f;
	float CrouchModifier = 1.f;
	float RunModifier = 1.f;
	float JumpModifier = 1.f;
	float MarksmanModifier = 1.f;

	// UPROPERTY()
	// TObjectPtr<UAmmoContainer> AmmunitionContainer;
	
	FName AmmunitionContainerSocket;
	
	UPROPERTY()
	TObjectPtr<UAnimationAsset> FireAnimation;

protected:

private:	
	/*
	 * Ammunition and Container
	*/
	UFUNCTION()
	void OnRep_Ammo();

	void SpendRound();
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Container")
	TObjectPtr<UAmmoContainer> AmmoContainer;

	UPROPERTY(EditAnywhere, Category="Weapon Properties|Container")
	TSubclassOf<ACasing> CasingClass;

	// TODO: Should be its own container class
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_Ammo)
	int32 Ammo;

	UPROPERTY(EditAnywhere, Category="Weapon Properties|Container")
	int32 MagCapacity = 8;
	/*
	 * Ammunition and Container - END
	 */

public:
	FORCEINLINE UAmmoContainer* GetAmmoContainer() const { return AmmoContainer; }
	FORCEINLINE float GetMarksmanFOV() const { return MarksmanFOV; }
	FORCEINLINE float GetMarksmanInterpSpeed() const { return MarksmanInterpSpeed; }
	bool HasAmmo();
	FORCEINLINE bool IsFull() const { return Ammo == MagCapacity; }
	FORCEINLINE int32 GetAmmo() const { return Ammo; } 
	FORCEINLINE int32 GetMagCapacity() const { return MagCapacity; } 
};