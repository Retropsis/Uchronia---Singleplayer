// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Data/GridInventoryData.h"
#include "UI/Widget/BaseUserWidget.h"
#include "GridInventory.generated.h"

class UBorder;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UGridInventory : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	void CreateLineSegments();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(EditAnywhere)
	int32 GridColumns = 10;
	
	UPROPERTY(EditAnywhere)
	int32 GridRows = 10;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UBorder> GridBorder;

	TArray<FGridLine> Lines;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TileSize = 100.f;

protected:
	virtual void NativeOnInitialized() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
		const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
};
