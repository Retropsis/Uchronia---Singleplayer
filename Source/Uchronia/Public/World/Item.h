// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "ItemInfo.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "Item.generated.h"

class APlayerCharacter;

UCLASS()
class UCHRONIA_API AItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AItem();

	//~ Interaction Interface
	virtual void Interact(APlayerCharacter* PlayerCharacter) override;
	//~ Interaction Interface

	
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Item Properties")
	TObjectPtr<UStaticMeshComponent> ItemMesh;
	
	UPROPERTY(EditAnywhere, Category="Item Properties")
	FItemStruct ItemData;

public:
	FORCEINLINE FItemStruct GetItemData() const { return ItemData; }
	virtual void Use(APlayerCharacter* PlayerCharacter) override;

	UFUNCTION(BlueprintCallable)
	void SetItemMesh();
};
