// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Data/ItemData.h"
#include "ItemBase.generated.h"

class UInventoryComponent;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	// Item Properties
	UPROPERTY()
	TObjectPtr<UInventoryComponent> OwningInventory;
	
	UPROPERTY(VisibleAnywhere, Category="Item") int32 Quantity;
	
	UPROPERTY(VisibleAnywhere, Category="Item") FName ID;
	UPROPERTY(VisibleAnywhere, Category="Item") FItemTextData TextData;
	UPROPERTY(VisibleAnywhere, Category="Item") EItemType ItemType;
	UPROPERTY(VisibleAnywhere, Category="Item") EItemCondition ItemCondition;
	UPROPERTY(VisibleAnywhere, Category="Item") FItemAssetData AssetData;
	UPROPERTY(VisibleAnywhere, Category="Item") FItemNumericData NumericData;
	UPROPERTY(VisibleAnywhere, Category="Item") FItemStatistics ItemStatistics;

	bool bIsCopy;
	bool bIsPickup;
	
	// Functions
	UItemBase();
	void ResetItemFlags();

	UFUNCTION(Category="Item")
	UItemBase* CreateItemCopy() const;

	UFUNCTION(Category="Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * NumericData.Weight; };
	UFUNCTION(Category="Item")
	FORCEINLINE float GetItemSingleWeight() const { return NumericData.Weight; };
	UFUNCTION(Category="Item")
	FORCEINLINE bool IsItemFullStack() const { return Quantity == NumericData.MaxStackSize; };
	UFUNCTION(Category="Item")
	FORCEINLINE bool IsItemFullCharge() const { return Quantity == NumericData.MaxCharges; };

	void SetQuantity(int32 NewQuantity);

	virtual void Use(APlayerCharacter* PlayerCharacter);

protected:
	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}
};
