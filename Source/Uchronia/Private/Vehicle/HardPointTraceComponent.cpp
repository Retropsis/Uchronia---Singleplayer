// Retropsis @ 2023-2024

#include "Vehicle/HardPointTraceComponent.h"
#include "Uchronia/Uchronia.h"
#include "Vehicle/SKM_ComponentCore.h"

void UHardPointTraceComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetCollisionObjectType(ECC_HardPoint);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionResponseToAllChannels(ECR_Overlap);
	ComponentTags.Add("HardPoint");
}

void UHardPointTraceComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
}

EHardPoint UHardPointTraceComponent::GetHardPoint_Implementation() const
{
	return HardPoint;
}

USkeletalMesh* UHardPointTraceComponent::GetComponentPreview_Implementation() const
{
	return HardPointComponent->GetSkeletalMeshAsset();
}

USKM_ComponentCore* UHardPointTraceComponent::GetHardPointComponent_Implementation() const
{
	return HardPointComponent;
}
