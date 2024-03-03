// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleCore.generated.h"

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
	UFUNCTION(BlueprintCallable) bool TickFuelConsumption(float QueriedAmount);
	UFUNCTION(BlueprintCallable) void WaterSplashAtLocation();

	UPROPERTY(BlueprintAssignable, Category="Vehicle|Components")
	FOnFuelEmpty OnFuelEmptyDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="Vehicle|Components")
	FOnFuelChange OnFuelChangeDelegate;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Vehicle")
	TObjectPtr<AVehicle> OwningVehicle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	TArray<TObjectPtr<AComponentCore>> Engines;

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

private:
	FTimerHandle FuelConsumptionTimer;

public:	
		
};
