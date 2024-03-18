// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "ComponentData.h"
#include "Interaction/HardPointInterface.h"
#include "Vehicle/TraceComponent.h"
#include "HardPointTraceComponent.generated.h"

class AVehicle;
class USKM_ComponentCore;
/**
 * 
 */
UCLASS()
class UCHRONIA_API UHardPointTraceComponent : public UTraceComponent, public IHardPointInterface
{
	GENERATED_BODY()

public:
	virtual void InitializeComponent() override;
	virtual void OnComponentCreated() override;
	virtual EHardPoint GetHardPoint_Implementation() const override;
	virtual USkeletalMesh* GetComponentPreview_Implementation() const override;
	virtual USKM_ComponentCore* GetHardPointComponent_Implementation() const override;

	UPROPERTY(BlueprintReadOnly, Category="Vehicle|HardPoint")
	EHardPoint HardPoint = EHardPoint::EHP_None;

	UPROPERTY(BlueprintReadWrite, Category="Vehicle|HardPoint")
	TObjectPtr<USKM_ComponentCore> HardPointComponent;
};
