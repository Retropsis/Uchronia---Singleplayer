// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "WorldInteractable_.generated.h"

class USphereComponent;

UCLASS()
class UCHRONIA_API AWorldInteractable_ : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AWorldInteractable_();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	virtual void Interact(APlayerCharacter* PlayerCharacter) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties")
	TObjectPtr<UStaticMeshComponent> InteractableMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties")
	TObjectPtr<USoundBase> InteractableSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties")
	bool bSoundShouldPropagate = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties")
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CollisionRadius = 100.f;
};
