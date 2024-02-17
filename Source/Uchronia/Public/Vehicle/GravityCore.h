// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GravityCore.generated.h"

UENUM()
enum class EGravityState : uint8
{
	EGS_Off UMETA(DisplayName="Off"),
	EGS_On UMETA(DisplayName="On"),
	EGS_Docked UMETA(DisplayName="Docked"),
	
	EGS_MAX UMETA(DisplayName="DefaultMAX")
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCHRONIA_API UGravityCore : public USceneComponent
{
	GENERATED_BODY()

public:	
	UGravityCore();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|GravityCore")
	EGravityState GravityState = EGravityState::EGS_Docked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|GravityCore")
	float SkimmingHeight = 150.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|GravityCore")
	float StabilityAmplitude = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|GravityCore")
	float StabilityFrequency = 5.f;

protected:
	virtual void BeginPlay() override;

public:	
};
