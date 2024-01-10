// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "World/WorldInteractable_.h"
#include "WorldItem_.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class UCHRONIA_API AWorldItem_ : public AWorldInteractable_
{
	GENERATED_BODY()

public:
	virtual void Interact(APlayerCharacter* PlayerCharacter) override;
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	UPROPERTY()
	TObjectPtr<UInventoryComponent> OwningInventory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties")
	FText ItemName = FText::GetEmpty();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties")
	FText ItemDescription = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties")
	TObjectPtr<UTexture2D> ItemIcon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties")
	TObjectPtr<UStaticMesh> ItemMesh;
};
