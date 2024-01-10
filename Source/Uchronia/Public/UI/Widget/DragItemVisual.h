// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "DragItemVisual.generated.h"

class UImage;
class UProgressBar;
class UTextBlock;
class UBorder;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UDragItemVisual : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category="Drag Item Visual", meta=(BindWidget))
	TObjectPtr<UBorder> ItemBorder;
	
	UPROPERTY(VisibleAnywhere, Category="Drag Item Visual", meta=(BindWidget))
	TObjectPtr<UImage> ItemIcon;
	
	UPROPERTY(VisibleAnywhere, Category="Drag Item Visual", meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;

	UPROPERTY(VisibleAnywhere, Category="Drag Item Visual", meta=(BindWidget))
	TObjectPtr<UProgressBar> ItemCharges;
};
