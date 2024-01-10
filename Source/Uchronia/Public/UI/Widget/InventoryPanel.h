// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "InventoryPanel.generated.h"

class UInventoryItemSlot;
class UInventoryComponent;
class APlayerCharacter;
class UTextBlock;
class UWrapBox;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UInventoryPanel : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void RefreshInventory();

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWrapBox> InventoryWrapBox;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> WeightInfo;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CapacityInfo;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;
	
protected:
	void SetInfoText() const;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
