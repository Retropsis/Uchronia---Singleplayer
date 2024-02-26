// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "VehicleData.h"
#include "GameFramework/Pawn.h"
#include "Vehicle.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UHullComponentCore;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGearChange, EGears, CurrentGear);

UCLASS()
class UCHRONIA_API AVehicle : public APawn
{
	GENERATED_BODY()

public:
	AVehicle();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category="Vehicle|Core")
	FOnGearChange OnGearChangeDelegate;
	
	UFUNCTION(BlueprintCallable)
	void SetNewGear(const EGears NewGear);
	
	UFUNCTION(BlueprintCallable)
	EGears GetCurrentGear() const { return CurrentGear; };
	
	UFUNCTION(BlueprintCallable)
	void IncreaseGear();
	
	UFUNCTION(BlueprintCallable)
	void DecreaseGear();

	UFUNCTION(BlueprintCallable)
	void SetThrustSpeedByGear(EGears InCurrentGear);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Core")
	TObjectPtr<UHullComponentCore> HullMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Core")
	float TargetThrustSpeed = 0.f;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Camera")
	float DefaultArmLength = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Camera")
	float FirstPersonArmLength = -80.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Core")
	EGears CurrentGear = EGears::EST_N;

private:

public:	
};
