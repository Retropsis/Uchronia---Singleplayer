// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseInventoryComponent.generated.h"

class AWorldItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCHRONIA_API UBaseInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseInventoryComponent();

	UFUNCTION(BlueprintCallable)
	void TryInteract();

	

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<AWorldItem> InteractingItem;
	
	TArray<TObjectPtr<AWorldItem>> OverlappingItems;
	
};
