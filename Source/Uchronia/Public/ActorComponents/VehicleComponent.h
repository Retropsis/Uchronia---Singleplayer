// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Vehicle/Vehicle.h"
#include "Vehicle/VehicleData.h"
#include "VehicleComponent.generated.h"

class UInputMappingContext;
class AVehicle;
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCHRONIA_API UVehicleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVehicleComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InitializeVehicle();

	// TODO: Should merge with WorldInteraction
	UFUNCTION(BlueprintCallable)
	void InteractWithVehicle();
	
	float InteractionDistance = 600.f;
	
	void TryEnterVehicle(AVehicle* VehicleToEnter);
	
	UFUNCTION(Server, Reliable)
	void ServerTryEnterVehicle(AVehicle* VehicleToEnter);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerExitVehicle();
	
	UFUNCTION(BlueprintCallable)
	void ExitVehicle();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttachPlayerToSeat(APlayerCharacter* PlayerToAttach, AVehicle* InVehicle, FName SocketName);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDetachPlayerFromSeat();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Vehicle|Input")
	TObjectPtr<UInputMappingContext>VehicleContext;
	
	UPROPERTY()
	TObjectPtr<APlayerCharacter> OwningCharacter = nullptr;

	UPROPERTY()
	TObjectPtr<AVehicle> InteractingVehicle = nullptr;

	ESeatType OccupyingSeat = ESeatType::EST_None;

	UPROPERTY(Replicated)
	bool bIsInVehicle;

public:	
	FORCEINLINE UInputMappingContext* GetVehicleContext() const { return VehicleContext; }
	FORCEINLINE void SetInteractingVehicle(AVehicle* InVehicle) { InteractingVehicle = InVehicle; }
};
