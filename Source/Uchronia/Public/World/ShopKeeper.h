// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "ItemInfo.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "ShopKeeper.generated.h"

UCLASS()
class UCHRONIA_API AShopKeeper : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AShopKeeper();
	virtual  void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Interact(APlayerCharacter* PlayerCharacter) override;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> ShopKeeperMesh;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<FItemStruct> InventoryItems;
	
};
