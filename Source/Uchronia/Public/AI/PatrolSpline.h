// Retropsis @ 2023-2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolSpline.generated.h"

class USplineComponent;

UCLASS()
class UCHRONIA_API APatrolSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	APatrolSpline();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI|Patrol")
	TObjectPtr<USplineComponent> PatrolSpline;
};
