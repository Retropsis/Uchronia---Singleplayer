// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "VehicleData.h"
#include "GameFramework/Pawn.h"
#include "Interaction/VehicleInterface.h"
#include "Vehicle.generated.h"

class USkimmerMovement;
class UVehicleCore;
class APlayerCharacter;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UHullComponentCore;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGearChange, EGears, CurrentGear);

UCLASS()
class UCHRONIA_API AVehicle : public APawn, public IVehicleInterface
{
	GENERATED_BODY()

public:
	AVehicle();
	virtual void Tick(float DeltaTime) override;
	virtual ESeatType GetFirstAvailableSeat_Implementation() override;
	virtual bool IsAnySeatAvailable_Implementation() override;
	virtual AVehicle* GetOwningVehicle_Implementation() override;
	UFUNCTION(BlueprintCallable) UVehicleCore* GetVehicleCore() const { return  VehicleCore; }

	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ServerExitVehicle();

	UFUNCTION(BlueprintCallable)
	virtual void OccupySeat(APlayerCharacter* PlayerCharacter, ESeatType Seat, bool bToOccupy);

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
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Core")
	TObjectPtr<UVehicleCore> VehicleCore;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Core")
	// TSubclassOf<UActorComponent> MovementComponentClass;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Core")
	// TObjectPtr<USkimmerMovement> SkimmerMovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Core")
	EGears CurrentGear = EGears::EST_N;
	
	/*
	 * Camera
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Camera")
	float DefaultArmLength = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Camera")
	float FirstPersonArmLength = -80.f;

	/*
	 * Input Mapping Context
	 */	
	UPROPERTY(EditDefaultsOnly, Category="Vehicle|Input")
	TObjectPtr<UInputMappingContext>VehicleControlsContext;
	
	UPROPERTY(EditDefaultsOnly, Category="Vehicle|Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Vehicle|Input")
	TObjectPtr<UInputAction> ExitAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Vehicle|Input")
	TObjectPtr<UInputAction> SwitchViewAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Vehicle|Input")
	TObjectPtr<UInputAction> SwitchGearAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Vehicle|Input")
	TObjectPtr<UInputAction> SwitchEngineAction;

	/*
	 * Seats
	 */
	UPROPERTY(BlueprintReadWrite)
	bool bDriverSeatOccupied = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool bPassengerSeatOccupied = false;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerCharacter> DriverCharacter;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerCharacter> PassengerCharacter;

private:
	void Look(const FInputActionValue& InputActionValue);
	void ExitVehicle(const FInputActionValue& InputActionValue);
	void SwitchCameraView(const FInputActionValue& InputActionValue);
	void SwitchGear(const FInputActionValue& InputActionValue);
	void SwitchEngine(const FInputActionValue& InputActionValue);
	bool bIsFirstPersonView = false;

public:
	FORCEINLINE UInputMappingContext* GetVehicleControlsContext() const { return  VehicleControlsContext; }
	FORCEINLINE UHullComponentCore* GetHullMesh() const { return  HullMesh; }
};
