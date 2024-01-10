// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "Pickup.generated.h"

class UItemBase;
class USphereComponent;

UCLASS()
class UCHRONIA_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	APickup();
	virtual void Destroyed() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	//~ Interaction Interface
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	//~ Interaction Interface
	
	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);
	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	//~ Interaction Interface
	virtual void Interact(APlayerCharacter* PlayerCharacter) override;
	//~ Interaction Interface

	void UpdateInteractableData();
	
	void TakePickup(const APlayerCharacter* Taker);
	
	UPROPERTY(EditInstanceOnly, Category="Pickup Properties | Initialization")
	FName DesiredItemID;
	
	UPROPERTY(EditInstanceOnly, Category="Pickup Properties | Initialization")
	TObjectPtr<UDataTable> ItemDataTable;
	
	UPROPERTY(EditInstanceOnly, Category="Pickup Properties | Initialization")
	int32 ItemQuantity;
	
	UPROPERTY(EditInstanceOnly, Category="Pickup Properties | Initialization")
	TObjectPtr<UStaticMeshComponent> PickupMesh;

	UPROPERTY(EditInstanceOnly, Category="Pickup Properties | Initialization")
	TObjectPtr<USoundBase> PickupSound;

	UPROPERTY(VisibleAnywhere, Category="Pickup Properties")
	TObjectPtr<UItemBase> ItemReference;

	UPROPERTY(VisibleInstanceOnly, Category="Pickup Properties")
	FInteractableData InstanceInteractableData;
	
private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerTaking;
	
	UPROPERTY(EditDefaultsOnly, Category="Pickup Properties")
	TObjectPtr<USphereComponent> OverlapSphere;
	
	UPROPERTY(EditDefaultsOnly, Category="Pickup Properties")
	float OverlapRadius = 100.f;
	
public:	
	FORCEINLINE UItemBase* GetItemData() const { return ItemReference; }
	
};
