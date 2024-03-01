// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleCore.generated.h"

class UFuelComponent;
class USKM_ComponentCore;
class AVehicle;
class AComponentCore;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class UCHRONIA_API UVehicleCore : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVehicleCore();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Vehicle")
	TObjectPtr<AVehicle> OwningVehicle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	TArray<TObjectPtr<AComponentCore>> Engines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	int32 EngineCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	TObjectPtr<UFuelComponent> FuelComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	TArray<TObjectPtr<USKM_ComponentCore>> FuelGauges;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Components")
	bool bIsRefueling = false;

private:

public:	
		
};
