// Retropsis @ 2023-2024

#include "AI/PatrolSpline.h"

#include "Components/SplineComponent.h"

APatrolSpline::APatrolSpline()
{
	PrimaryActorTick.bCanEverTick = false;

	PatrolSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolSpline"));
	SetRootComponent(PatrolSpline);
}
