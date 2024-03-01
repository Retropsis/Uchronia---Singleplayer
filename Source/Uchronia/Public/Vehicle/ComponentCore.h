// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ComponentCore.generated.h"

class AVehicle;

UCLASS()
class UCHRONIA_API AComponentCore : public AActor
{
	GENERATED_BODY()
	
public:	
	AComponentCore();
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AVehicle> OwningVehicle;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable) void SetOwningVehicle(AVehicle* InVehicle) { OwningVehicle = InVehicle; }
};
