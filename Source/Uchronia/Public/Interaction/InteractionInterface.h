// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class AWorldItem_;
class APlayerCharacter;

UENUM()
enum class EInteractableType : uint8
{
	EIT_Pickup UMETA(DisplayName="Pickup"),
	EIT_NonPlayableCharacter UMETA(DisplayName="NonPlayableCharacter"),
	EIT_Device UMETA(DisplayName="Device"),
	EIT_Toggle UMETA(DisplayName="Toggle"),
	EIT_Container UMETA(DisplayName="Container")
};

USTRUCT()
struct FInteractableData
{
	GENERATED_BODY()

	FInteractableData() :
	InteractableType(EInteractableType::EIT_Pickup),
	Name(FText::GetEmpty()),
	Action(FText::GetEmpty()),
	Quantity(0),
	Duration(0.0f)
	{
		
	};

	UPROPERTY(EditInstanceOnly) EInteractableType InteractableType;
	UPROPERTY(EditInstanceOnly) FText Name;
	UPROPERTY(EditInstanceOnly) FText Action;
	UPROPERTY(EditInstanceOnly) int8 Quantity;
	UPROPERTY(EditInstanceOnly) float Duration;
};

UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UCHRONIA_API IInteractionInterface
{
	GENERATED_BODY()

public:
	virtual void BeginFocus();
	virtual void EndFocus();
	virtual void BeginInteract();
	virtual void EndInteract();

	virtual void Interact(APlayerCharacter* PlayerCharacter) = 0;
	virtual void Use(APlayerCharacter* PlayerCharacter) {};
	virtual void AddItem(AWorldItem_* ItemToAdd) {};
	virtual void DropItem(TSubclassOf<AWorldItem_> ItemToDrop) {};
	
	FInteractableData InteractableData;
};
