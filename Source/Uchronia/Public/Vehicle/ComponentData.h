#pragma once

#include "CoreMinimal.h"
#include "ComponentData.generated.h"

UENUM(BlueprintType)
enum class EComponentInteractionType : uint8
{
	ECIT_Hatch UMETA(DisplayName="Hatch"),
	ECIT_FuelEntry UMETA(DisplayName="FuelEntry"),
	ECIT_FuelGauge UMETA(DisplayName="FuelGauge"),
	
	ECIT_Max UMETA(DisplayName="DefaultMAX")
};

UENUM(BlueprintType)
enum class EHardPoint : uint8
{
	EHP_None UMETA(DisplayName="None"),
	EHP_Hatch_Front_L UMETA(DisplayName="Hatch_Front_L"),
	EHP_Hatch_Front_R UMETA(DisplayName="Hatch_Front_R"),
	EHP_Hatch_Middle_L UMETA(DisplayName="Hatch_Middle_L"),
	EHP_Hatch_Middle_R UMETA(DisplayName="Hatch_Middle_R"),
	EHP_Hatch_Back_L UMETA(DisplayName="Hatch_Back_L"),
	EHP_Hatch_Back_R UMETA(DisplayName="Hatch_Back_R"),
	EHP_Helm UMETA(DisplayName="Helm"),
	EHP_Engine_L UMETA(DisplayName="Engine"),
	EHP_Lampost UMETA(DisplayName="Lampost"),
	EHP_Gauge UMETA(DisplayName="Gauge"),
	EHP_Moped_Hatch_L UMETA(DisplayName="Moped_Hatch_L"),
	EHP_Moped_Hatch_R UMETA(DisplayName="Moped_Hatch_R"),
	EHP_Wheel UMETA(DisplayName="Wheel"),
	
	EHP_MAX UMETA(DisplayName="DefaultMAX")
};

USTRUCT(BlueprintType)
struct FComponentData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component Data") FName ComponentName = FName();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component Data") TObjectPtr<UStaticMesh> ComponentMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component Data") TObjectPtr<UMaterialInstance> ComponentMaterialInstance = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component Data") TObjectPtr<USkeletalMesh> ComponentSkeletalMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component Data") TObjectPtr<USkeletalMesh> ComponentPreview = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component Data") TSubclassOf<UAnimInstance> ComponentAnimInstance = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component Data") float ComponentMass = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component Data") EHardPoint HardPoint = EHardPoint::EHP_None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component Data") FName HardPointSocketName = FName();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component Data") FName SmokeSocketName = FName();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Component Data") TSubclassOf<AActor> ComponentActor = nullptr;
	
};