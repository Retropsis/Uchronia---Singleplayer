// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkimmerMovement.generated.h"

class AVehicle;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCHRONIA_API USkimmerMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkimmerMovement();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Skimmer Movement")
	TObjectPtr<APawn> OwningVehicle;
	
	UPROPERTY(VisibleAnywhere, Category="Vehicle|Skimmer Movement")
	TObjectPtr<UPrimitiveComponent> HullReference;

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

public:	
};
