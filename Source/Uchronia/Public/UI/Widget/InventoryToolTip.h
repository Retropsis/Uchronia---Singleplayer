// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "InventoryToolTip.generated.h"

class UTextBlock;
class UInventoryItemSlot;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UInventoryToolTip : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInventoryItemSlot> InventorySlotBeingHovered;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemName;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemType;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemDescription;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemUsage;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> StackSize;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MaxStackSize;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> StackWeight;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemBuyValue;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemSellValue;
	
protected:
	virtual void NativeConstruct() override;

};
