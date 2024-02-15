// Retropsis @ 2023-2024

#include "Vehicle/HardPointTraceComponent.h"
#include "Uchronia/Uchronia.h"

EHardPoint UHardPointTraceComponent::GetHardPoint_Implementation() const
{
	return HardPoint;
}

void UHardPointTraceComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetCollisionObjectType(ECC_HardPoint);
}
