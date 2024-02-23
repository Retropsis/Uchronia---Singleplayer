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

UENUM(BlueprintType)
enum class ESeatType : uint8
{
	EST_Driver UMETA(DisplayName="Driver"),
	EST_Passenger_1 UMETA(DisplayName="Passenger_1"),
	EST_Passenger_2 UMETA(DisplayName="Passenger_2"),
	EST_Passenger_3 UMETA(DisplayName="Passenger_3"),
	EST_Passenger_4 UMETA(DisplayName="Passenger_4"),
	EST_MAX UMETA(DisplayName="DefaultMAX")
};

UENUM(BlueprintType)
enum class EGears : uint8
{
	EST_R UMETA(DisplayName="Reverse"),
	EST_N UMETA(DisplayName="Neutral"),
	EST_1 UMETA(DisplayName="1"),
	EST_2 UMETA(DisplayName="2"),
	EST_3 UMETA(DisplayName="3"),
	EST_4 UMETA(DisplayName="4"),
	EST_5 UMETA(DisplayName="5"),
	EST_6 UMETA(DisplayName="6"),
	EG_MAX UMETA(DisplayName="DefaultMAX")
};

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
