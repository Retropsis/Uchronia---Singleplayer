// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Vehicle.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UHullComponentCore;
class AHelm;
class UBoxComponent;

UCLASS()
class UCHRONIA_API AVehicle : public APawn
{
	GENERATED_BODY()

public:
	AVehicle();
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Core")
	TObjectPtr<UHullComponentCore> HullMesh;

protected:
	virtual void BeginPlay() override;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Player")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|Player")
	TObjectPtr<UCameraComponent> FollowCamera;

private:

public:	
};
