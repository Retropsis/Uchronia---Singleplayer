// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Actor/Weapon/AttachmentData.h"
#include "Actor/Weapon/WeaponData.h"
#include "Components/ActorComponent.h"
#include "HUD/PlayerHUD.h"
#include "Types/CombatState.h"
#include "CombatComponent.generated.h"

class UAttachmentComponent;
class AMeleeWeapon;
class ARangeWeapon;
class APlayerHUD;
class ACharacterPlayerController;
class UCharacterAnimInstance;
class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCHRONIA_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	friend class APlayerCharacter; // TODO: Could be base character ?
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateCarriedAmmo();
	void PlayEquipSound() const;

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(AWeapon* WeaponToEquip);

	UFUNCTION(BlueprintCallable)
	void InitializeWeapon();
	
	UFUNCTION(BlueprintCallable)
	AWeapon* SetupAttachments(TSubclassOf<AWeapon> WeaponToSetup, TMap<EAttachmentType, FAttachmentData> Attachments);
	
	UFUNCTION(BlueprintCallable)
	FAttachmentData UpdateAttachment(EAttachmentType AttachmentType, FAttachmentData NewAttachment);
	
	UFUNCTION(BlueprintCallable)
	void UpdateAttachments(TMap<EAttachmentType, FAttachmentData> Attachments);
	
	UFUNCTION(BlueprintCallable)
	void UnequipWeapon(AWeapon* WeaponToUnequip);
	
	void JumpToReloadEnd();

	UFUNCTION(BlueprintCallable)
	void ThrowStart();
	
	UFUNCTION(BlueprintCallable)
	void ThrowItem();
	
	UFUNCTION(BlueprintCallable)
	void ThrowEnd();

	UFUNCTION(BlueprintCallable)
	void MeleeStart();
	
	UFUNCTION(BlueprintCallable)
	void MeleeEnd();
	
	UFUNCTION(BlueprintCallable)
	void SetMeleeWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UFUNCTION(BlueprintCallable)
	void PickupAmmunition(EAmmunitionType AmmunitionType, int32 Amount);

protected:
	virtual void BeginPlay() override;
	void SetAiming(bool IsAiming);
	void Trigger(bool bPressed);
	void Fire();
	void Reload();
	void Throw();
	void DropEquippedWeapon() const;
	void ShowThrowableItem(bool bShowItem) const;

	void AttachActorToSocket(AActor* ActorToAttach, FName Socket) const;

	UFUNCTION(Server, Reliable)
	void ServerThrow();

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool IsAiming);

	UFUNCTION(Server, Reliable)
	void ServerTrigger(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastTrigger(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(Server, Reliable)
	void ServerReload();

	void HandleReload();

	UFUNCTION(BlueprintCallable)
	void AddSingleRound();

	UFUNCTION(BlueprintCallable)
	void ReloadEnd();

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
	void SetHUDCrosshairs(float DeltaTime);

	int32 AmountToReload();

private:
	TObjectPtr<APlayerCharacter> PlayerCharacter;
	UPROPERTY()
	ACharacterPlayerController* CharacterPlayerController;
	UPROPERTY()
	APlayerHUD* PlayerHUD;

	UPROPERTY(ReplicatedUsing=OnRep_EquippedWeapon)
	ARangeWeapon* EquippedWeapon;
	
	UPROPERTY(ReplicatedUsing=OnRep_EquippedMeleeWeapon)
	AMeleeWeapon* EquippedMeleeWeapon;

	UPROPERTY()
	UCharacterAnimInstance* CharacterAnimInstance;

	UFUNCTION()
	void OnRep_EquippedWeapon();
	
	UFUNCTION()
	void OnRep_EquippedMeleeWeapon();
	
	UPROPERTY(Replicated)
	bool bAiming = false;

	bool bTriggerButtonPressed;

	UPROPERTY(EditDefaultsOnly, Category="Combat Properties")
	float BaseWalkSpeed = 600.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat Properties")
	float AimWalkSpeed = 450.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat Properties")
	float TraceExtent = 100.f;

	/*
	 * HUD & Crosshair
	 */
	float CrosshairVelocityModifier;
	float CrosshairAirborneModifier;
	float CrosshairMarksmanModifier;
	float CrosshairRecoilModifier;
	
	float RunModifier = 1.f;
	float JumpModifier = 1.f;
	float MarksmanModifier = 1.f;	

	FVector HitTarget;
	
	FHUDPackage HUDPackage;
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Crosshairs")
	TObjectPtr<UTexture2D> Crosshair_Center;

	/*
	 * Marksman Mode
	 */
	// Set by Camera's default FoV
	float DefaultFOV;
	float CurrentFOV;

	/* TODO: To DataAsset */
	UPROPERTY(EditDefaultsOnly, Category="Combat Properties")
	float MarksmanFOV = 30.f;

	/* TODO: To DataAsset */
	UPROPERTY(EditDefaultsOnly, Category="Combat Properties")
	float MarksmanInterpSpeed = 20.f;

	UPROPERTY(EditDefaultsOnly, Category="Combat Properties")
	float BaseRecoverySpeed = 30.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat Properties")
	float BaseAirborneSpread = 2.25f;

	void InterpFOV(float DeltaTime);

	/*
	 * Automatic Fire
	 */
	FTimerHandle FireIntervalTimer;

	bool bCanFire = true;

	void FireIntervalStart();
	void FireIntervalEnd();
	bool CanFire();
	void InitializeCarriedAmmo();
	void UpdateAmmoValues();
	void UpdateSingleAmmoValue();
	void UpdateHUDGrenades();

	UPROPERTY(ReplicatedUsing=OnRep_CarriedAmmo)
	int32 CarriedAmmo;
	
	UFUNCTION()
	void OnRep_CarriedAmmo();

	TMap<EAmmunitionType, int32> CarriedAmmoMap;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxCarriedAmmo = 90;

	/*
	 * TODO: Probably for DEV ONLY
	 */
	UPROPERTY(EditDefaultsOnly)
	int32 Starting9mmAmmo = 16;
	
	UPROPERTY(EditDefaultsOnly)
	int32 StartingRocketAmmo = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 StartingLaserAmmo = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 StartingSubmachineGunAmmo = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 StartingShotgunAmmo = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 StartingHighCaliberRifleAmmo = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 StartingGrenadeLauncherAmmo = 0;
	
	UPROPERTY(ReplicatedUsing=OnRep_GrenadeCount)
	int32 GrenadeCount = 4;

	UFUNCTION()
	void OnRep_GrenadeCount();
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxGrenadeCount = 4;
	/*
	 *
	 */
	
	UPROPERTY(ReplicatedUsing=OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UFUNCTION()
	void OnRep_CombatState();
	
public:	

		
};
