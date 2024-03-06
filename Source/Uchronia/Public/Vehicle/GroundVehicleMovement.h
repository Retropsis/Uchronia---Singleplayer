// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Vehicle/VehicleMovement.h"
#include "GroundVehicleMovement.generated.h"

class UGroundSensingComponent;
/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCHRONIA_API UGroundVehicleMovement : public UVehicleMovement
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable) void InitializeGroundVehicleMovement(UVehicleCore* InVehicleCore);
	UFUNCTION(BlueprintCallable) void SolveMovement(float DeltaTime);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent) void SetHandleAngle(float NewAngle);
	virtual void SetThrustSpeedByGear(EGears InCurrentGear) override;

protected:
	/*
	 * Properties
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Properties")
	float MaxForwardSpeed = 320.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Properties")
	float MaxTurnSpeed = 180.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Properties")
	float HandleRotationSpeed = 50.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Properties")
	float HeightOffset = 35.f;
	
	/*
	 * Calculation
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Calculation")
	TArray<UGroundSensingComponent*> GroundSensors;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Calculation")
	FVector AverageNormal = FVector::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Calculation")
	float AverageHeight = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Calculation")
	int32 CountOnGround = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Calculation")
	float DeltaZ = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Calculation")
	float CurrentForwardSpeed = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Calculation")
	float TargetForwardSpeed = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Calculation")
	float TargetTurnSpeed = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Calculation")
	float CurrentHandleAngle = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Ground Movement|Calculation")
	float TargetHandleAngle = 0.f;
};
