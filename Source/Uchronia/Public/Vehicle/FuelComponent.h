// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FuelComponent.generated.h"

UENUM(BlueprintType)
enum class ERefuelState : uint8
{
	ERS_None UMETA(DisplayName="None"),
	ERS_In UMETA(DisplayName="In"),
	ERS_Out UMETA(DisplayName="Out"),
	ERS_Leak UMETA(DisplayName="Leak"),
	ERS_MAX UMETA(DisplayName="DefaultMAX")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class UCHRONIA_API UFuelComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFuelComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable) virtual void FuelTransferOperation(float DeltaTime);
	UFUNCTION(BlueprintCallable) virtual bool StartFuelTransferOperation(UFuelComponent* InTargetFuelComponent);
	UFUNCTION(BlueprintCallable) virtual void EndFuelTransferOperation();
	UFUNCTION(BlueprintCallable) float RetrieveFuel(float QueriedAmount);
	UFUNCTION(BlueprintCallable, BlueprintPure) bool IsFuelEntryCleared();
	UFUNCTION(BlueprintCallable) bool ShouldEndFuelTransfer();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	TObjectPtr<UFuelComponent> TargetFuelComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	float CurrentFuelQuantity = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	float MaxFuelCapacity = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	float FuelTransferRate = 0.1f;

private:
	bool bIsFuelLidOpen = false;
	ERefuelState RefuelState = ERefuelState::ERS_None;

	float CapacityTolerance = 0.0001f;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure) ERefuelState GetRefuelState() const { return RefuelState; }
	UFUNCTION(BlueprintCallable) void SetRefuelState(const ERefuelState NewFuelState) { RefuelState = NewFuelState; }
	UFUNCTION(BlueprintCallable, BlueprintPure) float GetCurrentFuelQuantity() const { return CurrentFuelQuantity; }
	UFUNCTION(BlueprintCallable) void SetCurrentFuelQuantity(const float NewQuantity) { CurrentFuelQuantity = NewQuantity; }
	UFUNCTION(BlueprintCallable, BlueprintPure) float GetMaxFuelCapacity() const { return MaxFuelCapacity; }
	UFUNCTION(BlueprintCallable, BlueprintPure) float GetFuelTransferRate() const { return FuelTransferRate; }
	UFUNCTION(BlueprintCallable, BlueprintPure) bool GetIsLidOpen() const { return bIsFuelLidOpen; }
	UFUNCTION(BlueprintCallable) void SetIsLidOpen(const bool IsOpen) { bIsFuelLidOpen = IsOpen; }
	
};
