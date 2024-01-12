// Retropsis @ 2023-2024

#include "UI/Widget/GridInventory.h"
#include "ActorComponents/Inventory/InventoryComponent.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Inventory/Data/GridInventoryData.h"

void UGridInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder))
	{
		const FVector2D NewSize{ GridColumns * TileSize, GridRows * TileSize };
		CanvasPanelSlot->SetSize(NewSize);
		CreateLineSegments();
	}
}

int32 UGridInventory::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	for (const FGridLine& Line : Lines)
	{
		FGeometry Geometry = GridBorder->GetCachedGeometry();
		FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
		UWidgetBlueprintLibrary::DrawLine(
			Context,
			Line.Start + USlateBlueprintLibrary::GetLocalTopLeft(Geometry),
			Line.End + USlateBlueprintLibrary::GetLocalTopLeft(Geometry),
			FLinearColor(0.5f,0.5f,0.5f,0.7f),
			true,
			0.5f
		);
	}
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

void UGridInventory::CreateLineSegments()
{
	for (int32 i = 0; i <= GridColumns; i++)
	{
		const int32 X = TileSize * i;
		const int32 Height = GridRows * TileSize;
		FGridLine Line = FGridLine{ FVector2D{ static_cast<float>(X) , 0.f }, FVector2D{ static_cast<float>(X), static_cast<float>(Height) } };
		Lines.Add(Line);
	}
	for (int32 i = 0; i <= GridRows; i++)
	{
		const int32 Y = TileSize * i;
		const int32 Width = GridColumns * TileSize;
		FGridLine Line = FGridLine{ FVector2D{ 0.f , static_cast<float>(Y) }, FVector2D{ static_cast<float>(Width), static_cast<float>(Y) } };
		Lines.Add(Line);
	}
}
