#pragma once

#include "CoreMinimal.h"
#include "AmmunitionData.h"
#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

#define TRACE_LENGTH 80000.f

USTRUCT(BlueprintType)
struct FReloadData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SectionName = FName("Pistol");
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 InternalCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsBoltAction = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanInterruptReload = false;
};

USTRUCT(BlueprintType)
struct FWeaponStatistics 
{
	GENERATED_BODY()

	//~ Base
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float MarksmanFOV = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float MarksmanInterpSpeed = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float DistanceToSphere = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float SphereRadius = 75.f;
	
	//~ Damage
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float FireInterval = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float HealthDamage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float HardDamage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float BloodLoss = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float DropoffRange = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float MinDamageRange = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float MaxRange = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float AccuracyRadiusAt10m = 0.f;
	
	//~ Aiming
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float PerShotModifier = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float ShotModifierCap = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float RecoveryDelay = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float RecoveryPerSecond = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float GravityModifier = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float CrouchModifier = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float WalkModifier = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float RunModifier = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float SprintModifier = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float JumpModifier = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float LeanModifier = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float MarksmanModifier = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float InVehicleModifier = 0.f;

	//~ Movement
};

USTRUCT(BlueprintType, Blueprintable)
struct FWeaponAssetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<USoundBase> EquipSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<USkeletalMesh> WeaponMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<UAnimationAsset> FireAnimation = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool bUsePhysicsAsset = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName PhysicsBoneName = FName();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<USkeletalMeshComponent> AmmunitionContainer = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName AmmunitionContainerSocket = FName();
};

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Debug") bool bVisualizeScatter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutomatic = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseScatter = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanAimDownSights = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FReloadData ReloadData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmunitionType AmmunitionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoContainerType AmmoContainerType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InternalMagazine;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InternalMagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponAssetData WeaponAssetData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponStatistics WeaponStatistics;
};
