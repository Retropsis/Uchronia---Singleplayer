// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FuelComponent.generated.h"

UENUM(BlueprintType)
enum class ERefuelState : uint8
{
	ERS_None UMETA(DisplayName="None"),
	ERS_In UMETA(DisplayName="In"),
	ERS_Out UMETA(DisplayName="Out"),
	ERS_Leak UMETA(DisplayName="Leak"),
	ERS_MAX UMETA(DisplayName="DefaultMAX")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCHRONIA_API UFuelComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFuelComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
