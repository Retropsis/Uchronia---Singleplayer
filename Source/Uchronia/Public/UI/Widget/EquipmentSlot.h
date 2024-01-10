// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Data/ItemData.h"
#include "UI/Widget/BaseUserWidget.h"
#include "EquipmentSlot.generated.h"

class UBorder;
enum class EItemType : uint8;
class UImage;
class UItemBase;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UEquipmentSlot : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	void RefreshSlot() const;
	
	FORCEINLINE void SetItemReference(UItemBase* Item) { ItemReference = Item; };
	FORCEINLINE UItemBase* GetItemReference() const { return ItemReference; }

	UPROPERTY(EditDefaultsOnly)
	EItemType SlotType = EItemType::EIT_Weapon;
	
	UPROPERTY()
	TObjectPtr<UItemBase> ItemReference;
	
	UPROPERTY(VisibleAnywhere, Category="Equipment Slot", meta=(BindWidget))
	TObjectPtr<UImage> ItemIcon;
	
	UPROPERTY(VisibleAnywhere, Category="Equipment Slot", meta=(BindWidget))
	TObjectPtr<UBorder> ItemBorder;

protected:
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
