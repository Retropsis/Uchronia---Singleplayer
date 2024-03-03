// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkimmerMovement.generated.h"

class UVehicleCore;
class AVehicle;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCHRONIA_API USkimmerMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkimmerMovement();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable) void InitializeSkimmerMovement(UVehicleCore* InVehicleCore);
	virtual void ToggleWaterSplashes();
	float GetVehicleElevation();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Skimmer Movement")
	TObjectPtr<APawn> OwningVehicle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Vehicle|Skimmer Movement")
	TObjectPtr<UPrimitiveComponent> HullReference;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Vehicle|Skimmer Movement")
	TObjectPtr<UVehicleCore> VehicleCore;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Calculation")
	FVector DeltaLocation = FVector::ZeroVector;

	/*
	 * VFX
	 */
	FTimerHandle WaterSplashTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|VFX")
	float WaterSplashCooldown = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|VFX")
	float WaterSplashThreshold = 100.f;

	bool bEnableWaterSplash = false;

private:
	UFUNCTION()
	void CallWaterSplash();

public:	
};
