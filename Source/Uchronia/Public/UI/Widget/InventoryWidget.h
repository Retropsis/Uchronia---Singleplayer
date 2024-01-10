// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "InventoryWidget.generated.h"

class UWrapBox;
class UInventoryComponent;
class APlayerCharacter;
class UInventorySlot;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UInventoryWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateInventory();
	
	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryReference;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWrapBox> InventoryWrapBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UInventorySlot> InventorySlotClass;

	UPROPERTY()
	TObjectPtr<UInventorySlot> InventorySlot;

protected:
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
