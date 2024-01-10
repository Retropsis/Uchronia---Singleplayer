// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "InterfaceTestActor.generated.h"

UCLASS()
class UCHRONIA_API AInterfaceTestActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AInterfaceTestActor();
	virtual void Tick(float DeltaTime) override;

	//~ Interaction Interface
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(APlayerCharacter* PlayerCharacter) override;
	//~ Interaction Interface

	UPROPERTY(EditInstanceOnly)
	FInteractableData InstanceInteractableData;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
};
