// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Components/ActorComponent.h"
#include "Interaction/InventoryInterface.h"
#include "Inventory/Data/GridInventoryData.h"
#include "InventoryComponent.generated.h"

class UItemData_;
class AWorldInteractable;
class APickup;
DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

class UItemBase;

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName="No Item Added"),
	IAR_PartialAmountItemAdded UMETA(DisplayName="Partial Amount Item Added"),
	IAR_AllItemAdded UMETA(DisplayName="All Item added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() : ActualAmountAdded(0), OperationResult(EItemAddResult::IAR_NoItemAdded), ResultMessage(FText::GetEmpty()) {};

	UPROPERTY(BlueprintReadOnly, Category="Inventory Component | Item Add Result")
	int32 ActualAmountAdded;
	
	UPROPERTY(BlueprintReadOnly, Category="Inventory Component | Item Add Result")
	EItemAddResult OperationResult;
	
	UPROPERTY(BlueprintReadOnly, Category="Inventory Component | Item Add Result")
	FText ResultMessage;

	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	};
	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	};
	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
	};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class UCHRONIA_API UInventoryComponent : public UActorComponent, public IInventoryInterface
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FOnInventoryUpdated OnInventoryUpdated;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	FItemAddResult HandleAddItem(UItemBase* ItemToAdd);
	
	UItemBase* FindMatchingItem(UItemBase* InItem) const;
	UItemBase* FindNextItemByID(UItemBase* InItem) const;
	UItemBase* FindNextPartialStack(UItemBase* InItem) const;
	
	void RemoveSingleInstanceOfItem(UItemBase* ItemToRemove);
	int32 RemoveAmountOfItem(UItemBase* ItemToRemove, int32 DesiredAmountToRemove);
	void SplitExistingStack(UItemBase* ItemToSplit, int32 AmountToSplit);

	UFUNCTION(Server, Reliable)
	void ServerDestroyPickup(APickup* PickupToDestroy);

	FORCEINLINE float GetInventoryTotalWeight() const { return InventoryTotalWeight; };
	FORCEINLINE float GetWeightCapacity() const { return InventoryWeightCapacity; };
	FORCEINLINE int32 GetSlotsCapacity() const { return InventorySlotsCapacity; };
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const { return InventoryContents; };

	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) { InventorySlotsCapacity = NewSlotsCapacity; };
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) { InventoryWeightCapacity = NewWeightCapacity; };

	FName GetItemID(const UItemBase* InItem) const { return InItem->ID; };
	UItemBase* FindItemByID(FName InItemID) const;

	void DropAllInventory();
	void DropItem(UItemBase* ItemToDrop, const int32 Quantity);
	
	/*
	 * Blueprint Callables
	 */	
	UPROPERTY(BlueprintReadWrite, Category="Inventory Component")
	TArray<TSubclassOf<AWorldInteractable>> Inventory;
	
	/*
	 * Equipment
	 */
	UFUNCTION(BlueprintCallable)
	void TryEquip(UItemBase* ItemToEquip, EItemType SlotType);
	virtual void TryEquip(UItemBase* ItemToEquip) override;
	virtual void TryEquipItem();
	UFUNCTION(BlueprintCallable)
	virtual void TryEquipWeapon(UItemBase* ItemToEquip);

	UPROPERTY(ReplicatedUsing=OnRep_EquippedMainHand)
	UClass* EquippedMainHand;

	UFUNCTION(Client, Reliable)
	void ClientSetEquippedMainHand(UClass* ItemToEquip);
	
	UFUNCTION(Server, Reliable)
	void ServerSetEquippedMainHand(UClass* ItemToEquip);

	UFUNCTION()
	void OnRep_EquippedMainHand();

	void AttachActorToSocket(UClass* ActorToAttach, const FName Socket) const;

	/*
	 * Grid
	 */
	UPROPERTY(EditAnywhere)
	int32 GridColumns = 10;
	
	UPROPERTY(EditAnywhere)
	int32 GridRows = 10;

	bool TryAddGridItem(UItemData_* ItemToAdd);

	bool IsRoomAvailable(UItemData_* ItemToCheck, int32 TopLeftIndex);
	FGridTile ForEachIndex(UItemData_* ItemToCheck, int32 TopLeftIndex);
	FGridTile InventoryIndexToTileXY(int32 Index);
	int32 TileXYToInventoryIndex(FGridTile TileCoords);
	bool IsTileValid(FGridTile Tile);
	bool GetItemAtIndex(int32 Index, UItemData_*& ItemFound);
	void AddItemAt(UItemData_* ItemToAdd, int32 TopLeftIndex);

	UPROPERTY()
	TArray<TObjectPtr<UItemData_>> Items;
	/*
	 * Grid
	 */
	
protected:
	virtual void BeginPlay() override;

	FItemAddResult HandleNonStackableItems(UItemBase* InItem);
	int32 HandleStackableItems(UItemBase* InItem, int32 RequestedAddAmount);
	int32 CalculateWeightAddAmount(UItemBase* InItem, int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount);

	void AddNewItem(UItemBase* Item, int32 AmountToAdd);

	UPROPERTY(VisibleAnywhere, Category="Inventory")
	float InventoryTotalWeight;
	
	UPROPERTY(EditInstanceOnly, Category="Inventory")
	float InventoryWeightCapacity;
	
	UPROPERTY(EditInstanceOnly, Category="Inventory")
	int32 InventorySlotsCapacity;

	UPROPERTY(VisibleAnywhere, Category="Inventory")
	TArray<TObjectPtr<UItemBase>> InventoryContents;
};