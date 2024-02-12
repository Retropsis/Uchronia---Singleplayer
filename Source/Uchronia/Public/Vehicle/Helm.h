// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "Helm.generated.h"

class AVehicle;

UCLASS()
class UCHRONIA_API AHelm : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AHelm();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(APlayerCharacter* PlayerCharacter) override;
	
	void UpdateInteractableData();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Helm Properties")
	AVehicle* VehicleReference;

	UPROPERTY(EditInstanceOnly, Category="Pickup Properties | Initialization")
	FName DesiredItemID;
	
	UPROPERTY(EditInstanceOnly, Category="Helm Properties")
	TObjectPtr<UDataTable> ItemDataTable;
	
	UPROPERTY(VisibleInstanceOnly, Category="Helm Properties")
	FInteractableData InstanceInteractableData;
	
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetVehicleReference(AVehicle* VehicleRef);
};
