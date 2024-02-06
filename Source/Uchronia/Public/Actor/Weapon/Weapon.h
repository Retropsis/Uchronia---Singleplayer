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
class AAmmoContainer;
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
	// void ShowPickupWidget(const bool bShowWidget) const;
	virtual void Drop();
	virtual void OnRep_Owner() override;
	
	/*
	 * DATA VARIABLES
	 * TODO: Move both CH and FoV stuff to Weapon DataAsset
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Properties")
	TObjectPtr<UDataTable> WeaponDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Properties")
	FDataTableRowHandle WeaponDataRow;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties")
	EWeaponType WeaponType;
	
	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties|Effects")
	TObjectPtr<USoundBase> EquipSound;
	
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
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
};
