// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "Inventory/Data/ItemData.h"
#include "WorldInteractable.generated.h"

class UInventoryComponent;
class USphereComponent;

UCLASS()
class UCHRONIA_API AWorldInteractable : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AWorldInteractable();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	void InitializeInteractable(const TSubclassOf<AWorldInteractable> BaseClass);
	virtual void UpdateInteractableData();
	
	UPROPERTY(EditDefaultsOnly, Category="Interactable Properties | Initialization")
	TObjectPtr<USphereComponent> OverlapSphere;
	
	UPROPERTY(EditInstanceOnly, Category="Interactable Properties | Initialization")
	FName DesiredItemID;
	
	UPROPERTY(EditInstanceOnly, Category="Interactable Properties | Initialization")
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interactable Properties | Initialization")
	TObjectPtr<UStaticMeshComponent> InteractableMesh;

	UPROPERTY(VisibleInstanceOnly, Category="Interactable Properties | Initialization")
	FInteractableData InstanceInteractableData;

	// TODO: can be hard coded again once properly derived
	UPROPERTY(EditAnywhere, Category="Interactable Properties | Initialization")
	EInteractableType InteractableType = EInteractableType::EIT_Container;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interactable Properties | Initialization")
	bool bUsePhysics = true;

	//~ Interaction Interface
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(APlayerCharacter* PlayerCharacter) override;
	//~ Interaction Interface

	// TODO: Can be a single function when BP_WorldItem is properly derived
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TryPickup(APlayerCharacter* PlayerCharacter);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void TryOpenContainer(APlayerCharacter* PlayerCharacter);

	UPROPERTY(VisibleAnywhere, Category="Interactable Properties | Initialization")
	TObjectPtr<AWorldInteractable> InteractableReference;

	/*
	 * Need this in WorldItem
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interactable Properties | World Item") int32 Quantity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interactable Properties | World Item") FName ID;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interactable Properties | World Item") FItemTextData TextData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interactable Properties | World Item") EItemType ItemType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interactable Properties | World Item") EItemCondition ItemCondition;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interactable Properties | World Item") FItemAssetData AssetData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interactable Properties | World Item") FItemNumericData NumericData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interactable Properties | World Item") FItemStatistics ItemStatistics;
	
	UFUNCTION(BlueprintCallable, Category="Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * NumericData.Weight; };
	UFUNCTION(BlueprintCallable, Category="Item")
	FORCEINLINE float GetItemSingleWeight() const { return NumericData.Weight; };
	UFUNCTION(BlueprintCallable, Category="Item")
	FORCEINLINE bool IsItemFullStack() const { return Quantity == NumericData.MaxStackSize; };
	UFUNCTION(BlueprintCallable, Category="Item")
	FORCEINLINE bool IsItemFullCharge() const { return Quantity == NumericData.MaxCharges; };
	UFUNCTION(BlueprintCallable, Category="Item")
	void SetQuantity(int32 NewQuantity);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Interacltable Properties")
	float OverlapRadius = 100.f;
};
