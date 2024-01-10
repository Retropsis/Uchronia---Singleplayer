// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "InventorySlot.generated.h"

class UInventoryToolTip_;
class UDragItemVisual;
class AWorldItem_;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UInventorySlot : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory Slot")
	TSubclassOf<UInventoryToolTip_> InventoryToolTipClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties", meta=(BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties", meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties")
	FLinearColor ItemCondition;
	
	UPROPERTY(VisibleAnywhere, Category="Inventory Slot")
	TObjectPtr<AWorldItem_> ItemReference;

	FORCEINLINE void SetItemReference(AWorldItem_* Item) { ItemReference = Item; };
	FORCEINLINE AWorldItem_* GetItemReference() const { return ItemReference; }
};
