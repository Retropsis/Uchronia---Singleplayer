// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "ComponentData.h"
#include "Interaction/HardPointInterface.h"
#include "Vehicle/TraceComponent.h"
#include "HardPointTraceComponent.generated.h"

class USKM_ComponentCore;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UHardPointTraceComponent : public UTraceComponent, public IHardPointInterface
{
	GENERATED_BODY()

public:
	virtual EHardPoint GetHardPoint_Implementation() const override;
	virtual void InitializeComponent() override;
	virtual void OnComponentCreated() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Vehicle|HardPoint")
	EHardPoint HardPoint = EHardPoint::EHP_None;

	UPROPERTY(EditAnywhere,  BlueprintReadWrite, Category="Vehicle|HardPoint")
	TObjectPtr<USKM_ComponentCore> HardPointComponent;

	
};
