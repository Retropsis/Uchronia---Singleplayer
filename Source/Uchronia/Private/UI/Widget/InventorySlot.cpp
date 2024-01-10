// Retropsis @ 2023-2024


#include "UI/Widget/InventorySlot.h"
#include "Components/Image.h"
#include "UI/Widget/DragItemVisual.h"
#include "UI/Widget/InventoryToolTip_.h"
#include "UI/Widget/ItemDragDropOperation.h"
#include "UI/Widget/ItemDragDropOperation_.h"
#include "World/WorldItem_.h"

void UInventorySlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	checkf(InventoryToolTipClass, TEXT("InventoryToolTipClass is missing, please fill up WBP_InventoryItemSlot"));
	
	UInventoryToolTip_* ToolTip = CreateWidget<UInventoryToolTip_>(this, InventoryToolTipClass);
	ToolTip->InventorySlotBeingHovered = this;
	SetToolTip(ToolTip);
}

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(ItemReference)
	{
		ItemIcon->SetBrushFromTexture(ItemReference->ItemIcon);
		// if(ItemReference->Item)
		// {
		// 	ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
		// }
		// else
		// {
		// 	ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		// }
	}
}

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	return Reply.Unhandled();
}

void UInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	checkf(DragItemVisualClass, TEXT("DragItemVisualClass is missing, please fill up WBP_InventoryItemSlot"));
	const TObjectPtr<UDragItemVisual> DragItemVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
	DragItemVisual->ItemIcon->SetBrushFromTexture(ItemReference->ItemIcon);
	// DragItemVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());
	
	// ItemReference->NumericData.bIsStackable
	// 	? DragItemVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity))
	// 	:  DragItemVisual->ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
	
	// DragItemVisual->ItemCharges->SetPercent(0.5f); 
	
	UItemDragDropOperation_* DragItemOperation = NewObject<UItemDragDropOperation_>();
	DragItemOperation->SourceItem = ItemReference;
	DragItemOperation->SourceInventory = ItemReference->OwningInventory;
	DragItemOperation->DefaultDragVisual = DragItemVisual;
	DragItemOperation->Pivot = EDragPivot::MouseDown;
	
	OutOperation = DragItemOperation;
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
