// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Components/ActorComponent.h"
#include "Interaction/InteractionInterface.h"
#include "InventoryComponent.generated.h"

class AWorldItem_;
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
class UCHRONIA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FOnInventoryUpdated OnInventoryUpdated;

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
	void TryEquip(UItemBase* ItemToEquip, EItemType SlotType);

	UPROPERTY(ReplicatedUsing=OnRep_EquippedMainHand)
	UClass* EquippedMainHand;

	UFUNCTION(Client, Reliable)
	void ClientSetEquippedMainHand(UClass* ItemToEquip);
	
	UFUNCTION(Server, Reliable)
	void ServerSetEquippedMainHand(UClass* ItemToEquip);

	UFUNCTION()
	void OnRep_EquippedMainHand();

	void AttachActorToSocket(UClass* ActorToAttach, const FName Socket) const;
	
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

public:
	/*
	 * T4
	 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlayerCharacter> PlayerCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Inventory")
	TArray<TSubclassOf<AWorldItem_>> Inventory_;

	FORCEINLINE TArray<TSubclassOf<AWorldItem_>> GetInventory_() const { return  Inventory_; }

	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(AWorldItem_* ItemToAdd, int32 AmountToAdd = 1);

	UFUNCTION(Server, Unreliable)
	void ServerPlayerSound(USoundBase* InSound);
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayerSound(USoundBase* InSound);
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerDestroyActor(AActor* ActorToDestroy);
	
	UFUNCTION(Server, Reliable)
	void ServerSpawnIem(TSubclassOf<AWorldItem_> ItemToSpawn, FTransform SpawnTransform);

	// Not used anymore
	UFUNCTION(Client, Reliable)
	void ClientSpawnIem(TSubclassOf<AWorldItem_> ItemToSpawn, FTransform SpawnTransform);
	
	void DropItemFromInventory(TSubclassOf<AWorldItem_> ItemToDrop);

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	float ItemSpawnDistance = 150.f;
};
