// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "VehicleData.h"
#include "VehicleMovement.h"
#include "Components/ActorComponent.h"
#include "SkimmerMovement.generated.h"

class UHullComponentCore;
enum class EGears : uint8;
class UVehicleCore;
class AVehicle;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCHRONIA_API USkimmerMovement : public UVehicleMovement
{
	GENERATED_BODY()

public:	
	USkimmerMovement();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable) void InitializeSkimmerMovement(UVehicleCore* InVehicleCore);
	UFUNCTION(BlueprintCallable) void SolveMovement(float DeltaTime);
	virtual void SetThrustSpeedByGear(EGears InCurrentGear) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Properties")
	float ThrustSpeedFactor = 600.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Properties")
	float SteeringSpeedFactor = 300.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Properties")
	float ElevationSpeedFactor = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Properties")
	float YawRotationSpeedFactor = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Properties")
	float ThrustRollFactor = 0.016667f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Properties")
	float SteerPitchFactor = -0.016667f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Calculation")
	float TargetSteeringSpeed = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Calculation")
	float TargetThrustSpeed = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Calculation")
	float TargetElevationSpeed = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Calculation")
	float TargetYawRotationSpeed = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Calculation")
	float CurrentSteeringSpeedFactor = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Calculation")
	float CurrentElevationSpeedFactor = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Calculation")
	float CurrentYawRotationSpeedFactor = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Calculation")
	float CurrentThrustSpeedFactor = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Calculation")
	FVector DeltaLocation = FVector::ZeroVector;

private:

public:	
};
