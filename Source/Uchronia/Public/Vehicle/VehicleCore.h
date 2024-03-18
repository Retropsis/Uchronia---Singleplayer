// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "VehicleData.h"
#include "Components/ActorComponent.h"
#include "VehicleCore.generated.h"

enum class EGears : uint8;
class UNiagaraSystem;
class UFuelComponent;
class USKM_ComponentCore;
class AVehicle;
class AComponentCore;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFuelEmpty, float, FuelModifier);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFuelChange);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class UCHRONIA_API UVehicleCore : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVehicleCore();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable) void InitializeVehicleCore();
	UFUNCTION(BlueprintCallable) void ToggleEngines(const bool ShouldActivate);
	UFUNCTION(BlueprintCallable) void UpdateFuelGauges();
	UFUNCTION(BlueprintCallable) void UpdateFuelStatus();
	UFUNCTION(BlueprintCallable) bool ShouldTickFuelConsumption();
	UFUNCTION(BlueprintCallable) bool TickFuelConsumption(float QueriedAmount);
	UFUNCTION(BlueprintCallable) void WaterSplashAtLocation();
	UFUNCTION(BlueprintCallable) bool TryAddingEngine(AComponentCore* Engine);
	UFUNCTION(BlueprintCallable) bool TryRemovingEngine(AComponentCore* Engine);
	UFUNCTION(BlueprintCallable) bool TryAddingWheel(USKM_ComponentCore* Wheel);
	UFUNCTION(BlueprintCallable) bool TryAddingFuelGauge(USKM_ComponentCore* FuelGauge);
	
	UFUNCTION() void HandleOnFuelEmptied(float InFuelModifier);
	UFUNCTION() void HandleOnGearChanged(EGears NewGear);
	void SetFuelTickConsumptionByGear();
	void SolveMovementState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Properties")
	bool bEngineRequirement = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Properties")
	bool bAllWheelsRequirement = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Properties")
	int32 WheelRequirementCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Properties")
	bool bSailRequirement = false;

	EGears CurrentGear = EGears::EST_N;

	UPROPERTY(BlueprintAssignable, Category="Vehicle|Components")
	FOnFuelEmpty OnFuelEmptyDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="Vehicle|Components")
	FOnFuelChange OnFuelChangeDelegate;

protected:
	virtual void BeginPlay() override;
	virtual void ToggleWaterSplashes();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Vehicle")
	TObjectPtr<AVehicle> OwningVehicle;
	
	EMovementState MovementState = EMovementState::EMS_Engine_Off_Ready;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	TArray<TObjectPtr<AComponentCore>> Engines;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	TArray<TObjectPtr<USKM_ComponentCore>> Wheels;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	TArray<TObjectPtr<AComponentCore>> Sails;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	int32 EngineCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	TObjectPtr<UFuelComponent> FuelComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	TArray<TObjectPtr<USKM_ComponentCore>> FuelGauges;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	bool bIsOutOfFuel = false;
	
	bool bLastIsOutOfFuel = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	bool bIsRefueling = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	float FuelConsumptionCooldown = 1.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|VFX")
	TArray<FName> WaterSplashesSockets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|VFX")
	TObjectPtr<UNiagaraSystem> WaterSplash = nullptr;

	/*
	 * VFX
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Skimmer Movement|VFX")
	float WaterSplashCooldown = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Skimmer Movement|VFX")
	float WaterSplashThreshold = 100.f;

private:
	FTimerHandle FuelConsumptionTimer;
	float CurrentFuelTickConsumption = 0.f;
	float FuelModifier = 0.f;

	/*
	 * VFX
	 */
	FTimerHandle WaterSplashTimer;
	bool bEnableWaterSplash = false;

public:	
		FORCEINLINE int32 GetEngineData() const { return Engines.Num(); }
		FORCEINLINE EMovementState GetMovementState() const { return MovementState; }
		FORCEINLINE void SetMovementState(const EMovementState NewState) { MovementState = NewState; }
};
