// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "WeaponTypes.h"
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void ShowPickupWidget(const bool bShowWidget) const;
	virtual void Drop();
	virtual void OnRep_Owner() override;
	
	/*
	 * TODO: Move both CH and FoV stuff to Weapon DataAsset
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
	 * Sound
	 */
	/* TODO: To DataAsset */
	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties|Effects")
	TObjectPtr<USoundBase> EquipSound;

	/*
	 *
	 */
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Rotation Correction") 
	float RightHandRotationRoll = -90.f;
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Rotation Correction") 
	float RightHandRotationYaw = 0.f;
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties|Rotation Correction") 
	float RightHandRotationPitch = -90.f;
	
protected:
	virtual void BeginPlay() override;
	void CauseDamage(const FHitResult& Hit);

	UFUNCTION()
	virtual  void OnSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);

	UFUNCTION()
	virtual  void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	
	UPROPERTY()
	APlayerCharacter* OwnerCharacter;
	
	UPROPERTY()
	ACharacterPlayerController* OwnerController;
	
	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

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

private:
	UPROPERTY(ReplicatedUsing=OnRep_WeaponState, VisibleAnywhere, Category="Weapon Properties")
	EWeaponState WeaponState;

	UFUNCTION()
	void OnRep_WeaponState();

	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	TObjectPtr<USphereComponent> OverlapSphere;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties")
	float OverlapSphereRadius = 100.f;

	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	TObjectPtr<UWidgetComponent> PickupWidget;

	UPROPERTY(EditDefaultsOnly, Category="Weapon Properties")
	EWeaponType WeaponType;
	/*  */
	
public:
	void SetWeaponState(const EWeaponState InWeaponState);
	FORCEINLINE USphereComponent* GetOverlapSphere() const { return OverlapSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
};
