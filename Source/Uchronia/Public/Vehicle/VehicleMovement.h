// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleMovement.generated.h"

enum class EGears : uint8;
class UVehicleCore;
class UHullComponentCore;
class AVehicle;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCHRONIA_API UVehicleMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVehicleMovement();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION() virtual void SetThrustSpeedByGear(EGears CurrentGear) {}

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Skimmer Movement")
	TObjectPtr<AVehicle> OwningVehicle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Vehicle|Skimmer Movement")
	TObjectPtr<UHullComponentCore> HullReference;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Vehicle|Skimmer Movement")
	TObjectPtr<UVehicleCore> VehicleCore;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vehicle|Skimmer Movement|Properties")
	FVector Gravity = FVector(0.f, 0.f, -20.f);

public:	

		
};
