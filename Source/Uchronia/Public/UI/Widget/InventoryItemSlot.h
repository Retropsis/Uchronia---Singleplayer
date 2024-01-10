// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "InventoryItemSlot.generated.h"

class UProgressBar;
class UTextBlock;
class UImage;
class UBorder;
class UInventoryToolTip;
class UDragItemVisual;
class UItemBase;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UInventoryItemSlot : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetItemReference(UItemBase* Item) { ItemReference = Item; };
	FORCEINLINE UItemBase* GetItemReference() const { return ItemReference; }

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UPROPERTY(EditDefaultsOnly, Category="Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory Slot")
	TSubclassOf<UInventoryToolTip> InventoryToolTipClass;
	
	UPROPERTY(VisibleAnywhere, Category="Inventory Slot")
	TObjectPtr<UItemBase> ItemReference;

	UPROPERTY(VisibleAnywhere, Category="Inventory Slot", meta=(BindWidget))
	TObjectPtr<UBorder> ItemBorder;
	
	UPROPERTY(VisibleAnywhere, Category="Inventory Slot", meta=(BindWidget))
	TObjectPtr<UImage> ItemIcon;
	
	UPROPERTY(VisibleAnywhere, Category="Inventory Slot", meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;

	UPROPERTY(VisibleAnywhere, Category="Inventory Slot", meta=(BindWidget))
	TObjectPtr<UProgressBar> ItemCharges;

	UPROPERTY(EditAnywhere)
	FLinearColor PristineColor;
};
