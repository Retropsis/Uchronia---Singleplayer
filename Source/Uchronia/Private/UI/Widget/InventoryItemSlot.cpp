// Retropsis @ 2023-2024

#include "UI/Widget/InventoryItemSlot.h"
#include "ActorComponents/Inventory/ItemBase.h"
#include "UI/Widget/InventoryToolTip.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/Widget/DragItemVisual.h"
#include "UI/Widget/ItemDragDropOperation.h"

void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	checkf(InventoryToolTipClass, TEXT("InventoryToolTipClass is missing, please fill up WBP_InventoryItemSlot"));
	
	UInventoryToolTip* ToolTip = CreateWidget<UInventoryToolTip>(this, InventoryToolTipClass);
	ToolTip->InventorySlotBeingHovered = this;
	SetToolTip(ToolTip);
}

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if(ItemReference)
	{
		switch (ItemReference->ItemCondition) {
		case EItemCondition::EIC_Pristine:
			ItemBorder->SetBrushColor(FLinearColor::Green);
			break;
		case EItemCondition::EIC_Worn:
			ItemBorder->SetBrushColor(FLinearColor(0.f, 1.f, 0.45f));
			break;
		case EItemCondition::EIC_Damaged:
			ItemBorder->SetBrushColor(FLinearColor::Yellow);
			break;
		case EItemCondition::EIC_BadlyDamaged:
			ItemBorder->SetBrushColor(FLinearColor(1.f, 0.45f, 0.f));
			break;
		case EItemCondition::EIC_Ruined:
			ItemBorder->SetBrushColor(FLinearColor::Red);
			break;
		default: ;
		}
		ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
		if(ItemReference->NumericData.bIsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
		}
		else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
		ItemCharges->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	return Reply.Unhandled();
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	checkf(DragItemVisualClass, TEXT("DragItemVisualClass is missing, please fill up WBP_InventoryItemSlot"));
	const TObjectPtr<UDragItemVisual> DragItemVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
	DragItemVisual->ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
	DragItemVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

	ItemReference->NumericData.bIsStackable
		? DragItemVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity))
		:  DragItemVisual->ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
	
	DragItemVisual->ItemCharges->SetPercent(0.5f); 

	UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
	DragItemOperation->SourceItem = ItemReference;
	DragItemOperation->SourceInventory = ItemReference->OwningInventory;
	DragItemOperation->DefaultDragVisual = DragItemVisual;
	DragItemOperation->Pivot = EDragPivot::MouseDown;

	OutOperation = DragItemOperation;
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
