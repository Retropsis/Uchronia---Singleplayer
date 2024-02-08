// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "WeaponData.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UGameplayEffect;
class AProjectile;
class ACharacterPlayerController;
class APlayerCharacter;
class ACasing;
class UAmmoContainer;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName="Initial"),
	EWS_Equipped UMETA(DisplayName="Equipped"),
	EWS_Dropped UMETA(DisplayName="Dropped"),

	EWS_MAX UMETA(DisplayName="DefaultMAX"),
};

class USphereComponent;

UCLASS()
class UCHRONIA_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual  void OnConstruction(const FTransform& Transform) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Drop();
	virtual void OnRep_Owner() override;
	
	/*
	 * DATA VARIABLES
	 * TODO: Move both CH and FoV stuff to Weapon DataAsset
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Properties")
	FDataTableRowHandle WeaponDataRow;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties")
	EAmmunitionType AmmunitionType;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties")
	EAmmoContainerType AmmoContainerType;
	
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
	
	UPROPERTY()
	TObjectPtr<USoundBase> EquipSound;

	FName ReloadSectionName = FName();
	bool bUsePhysicsAsset = false;
	bool bCanAimDownSights = false;
	bool bCanInterruptReload = false;
	
	/*
	 * Damage
	 */
	// TODO: Move this to DataAsset
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Container")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Properties|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties|Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
	/*
	 * Damage - END
	 */
	
	/*
	 * Crosshair Textures
	 */
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Crosshairs")
	TObjectPtr<UTexture2D> Crosshair_Center;
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Crosshairs")
	TObjectPtr<UTexture2D> Crosshair_Left;
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Crosshairs")
	TObjectPtr<UTexture2D> Crosshair_Top;
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Crosshairs")
	TObjectPtr<UTexture2D> Crosshair_Right;
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Crosshairs")
	TObjectPtr<UTexture2D> Crosshair_Bottom;

	/*
	 * 
	 */
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Rotation Correction") 
	float RightHandRotationRoll = -90.f;
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Rotation Correction") 
	float RightHandRotationYaw = 0.f;
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Rotation Correction") 
	float RightHandRotationPitch = -90.f;
	/*
	 * DATA END
	 */ 
	
protected:
	virtual void BeginPlay() override;
	void CauseDamage(const FHitResult& Hit);
	
	UPROPERTY()
	APlayerCharacter* OwnerCharacter;
	
	UPROPERTY()
	ACharacterPlayerController* OwnerController;

private:	
	UPROPERTY(ReplicatedUsing=OnRep_WeaponState, VisibleAnywhere, Category="Weapon Properties")
	EWeaponState WeaponState;

	UFUNCTION()
	void OnRep_WeaponState();
	/*  */
	
public:
	void SetWeaponState(const EWeaponState InWeaponState);
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE EAmmunitionType GetAmmunitionType() const { return AmmunitionType; }
	FORCEINLINE EAmmoContainerType GetAmmoContainerType() const { return AmmoContainerType; }
	FORCEINLINE FName GetReloadSectionName() const { return ReloadSectionName; }
	FORCEINLINE bool ShouldUsePhysicsAsset() const { return bUsePhysicsAsset; }
	FORCEINLINE bool CanAimDownSights() const { return bCanAimDownSights; }
	FORCEINLINE bool CanInterruptReload() const { return bCanInterruptReload; }
};
