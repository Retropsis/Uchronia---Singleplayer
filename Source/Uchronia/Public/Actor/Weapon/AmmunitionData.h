#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AmmunitionData.generated.h"

class AProjectile;

UENUM(BlueprintType)
enum class EAmmunitionType : uint8
{
	EAT_None UMETA(DisplayName="None"),
	EAT_Melee UMETA(DisplayName="Melee"),
	
	EAT_9x19mm UMETA(DisplayName="9x19mm"),
	EAT_357 UMETA(DisplayName=".357"),
	EAT_45 UMETA(DisplayName=".45"),
	EAT_308 UMETA(DisplayName=".308"),
	EAT_380 UMETA(DisplayName=".380"),
	EAT_22 UMETA(DisplayName=".22"),
	EAT_12ga UMETA(DisplayName="12ga"),
	EAT_556x45mm UMETA(DisplayName="556x45mm"),
	EAT_556x39mm UMETA(DisplayName="556x39mm"),
	EAT_762x39mm UMETA(DisplayName="762x39mm"),
	EAT_762x54mm UMETA(DisplayName="762x54mm"),
	EAT_40mm UMETA(DisplayName="40mm"),
	EAT_M6A1 UMETA(DisplayName="M6A1 Rocket"),
	EAT_Energy UMETA(DisplayName="Energy"),
	EAT_M67 UMETA(DisplayName="M67"),

	EAT_MAX UMETA(DisplayName="DefaultMAX")
};

UENUM(BlueprintType)
enum class EAmmoContainerType : uint8
{
	EACT_Internal UMETA(DisplayName="Internal"),
	EACT_Box UMETA(DisplayName="Box"),
	EACT_Magazine UMETA(DisplayName="Magazine"),
	EACT_Clip UMETA(DisplayName="Clip"),
	EACT_Barrel UMETA(DisplayName="Magazine"),
	EACT_Drum UMETA(DisplayName="Drum"),
	EACT_Battery UMETA(DisplayName="Battery"),
	
	EACT_MAX UMETA(DisplayName="DefaultMAX")
};

USTRUCT(BlueprintType)
struct FAmmunitionData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 CurrentCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxCapacity = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EAmmoContainerType AmmoContainerType = EAmmoContainerType::EACT_Magazine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EAmmunitionType AmmunitionType = EAmmunitionType::EAT_9x19mm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TObjectPtr<USkeletalMesh> ContainerMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ContainerSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<USoundBase> EmptyContainerSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<USoundBase> InsertContainerSound = nullptr;
};