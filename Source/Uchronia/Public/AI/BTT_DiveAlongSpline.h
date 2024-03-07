// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_DiveAlongSpline.generated.h"

/**
 * 
 */
UCLASS()
class UCHRONIA_API UBTT_DiveAlongSpline : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
