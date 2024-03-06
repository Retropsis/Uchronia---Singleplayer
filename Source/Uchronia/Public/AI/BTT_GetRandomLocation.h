// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_GetRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class UCHRONIA_API UBTT_GetRandomLocation : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
