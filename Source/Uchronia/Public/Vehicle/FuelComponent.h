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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	TObjectPtr<UFuelComponent> TargetFuelComponent = nullptr;

private:
	float CurrentFuelQuantity = 0.f;
	float MaxFuelCapacity = 10.f;
	float FuelTransferRate = 0.1f;
	bool IsFuelLidOpen = false;
	ERefuelState RefuelState = ERefuelState::ERS_None;

public:
	UFUNCTION(BlueprintCallable) void SetRefuelState(const ERefuelState NewFuelState) { RefuelState = NewFuelState; }
	UFUNCTION(BlueprintCallable) float GetCurrentFuelQuantity() { return CurrentFuelQuantity; }
	
};
