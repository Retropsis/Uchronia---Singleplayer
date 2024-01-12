// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item_.generated.h"

class UItemData_;
class USphereComponent;

UCLASS()
class UCHRONIA_API AItem_ : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem_();

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CollisionRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UItemData_> ItemData;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UItemData_* GetDefaultItemData();
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
