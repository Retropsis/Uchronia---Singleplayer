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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;
	virtual void Trigger(const FVector& HitTarget);
	void AddRounds(int32 RoundsToAdd);
	void SetHUDAmmo();
	
	/*
	 * Automatic Fire
	 */
	/* TODO: To DataAsset */
	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties|Combat")
	float FireInterval = .15f;

	/* TODO: To DataAsset */
	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties|Combat")
	bool bAutomatic = false;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties|Container")
	TObjectPtr<USoundBase> EmptyContainerSound;

	UPROPERTY(EditAnywhere)
	bool bVisualizeScatter = false;

protected:

private:
	/*
	 * TODO: Move to DataAsset
	*/
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Range Weapon")
	TObjectPtr<UAnimationAsset> FireAnimation;
	
	/*
	 * Ammunition and Container
	*/
	UFUNCTION()
	void OnRep_Ammo();

	void SpendRound();
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Container")
	TObjectPtr<AAmmoContainer> AmmoContainer;

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
	
	/*
	 * Marksman Mode
	 */
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Marksman Mode")
	float MarksmanFOV = 30.f;

	UPROPERTY(EditAnywhere, Category="Weapon Properties|Marksman Mode")
	float MarksmanInterpSpeed = 20.f;
	/*
	 * Marksman Mode - END
	*/

public:
	FORCEINLINE AAmmoContainer* GetAmmoContainer() const { return AmmoContainer; }
	FORCEINLINE float GetMarksmanFOV() const { return MarksmanFOV; }
	FORCEINLINE float GetMarksmanInterpSpeed() const { return MarksmanInterpSpeed; }
	FORCEINLINE bool HasAmmo() const { return Ammo > 0; }
	FORCEINLINE bool IsFull() const { return Ammo == MagCapacity; }
	FORCEINLINE int32 GetAmmo() const { return Ammo; } 
	FORCEINLINE int32 GetMagCapacity() const { return MagCapacity; } 
};