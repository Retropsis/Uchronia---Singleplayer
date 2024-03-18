// Retropsis @ 2023-2024


#include "Vehicle/SKM_ComponentCore.h"

#include "NiagaraFunctionLibrary.h"
#include "Engine/SkinnedAssetCommon.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Vehicle/ComponentCore.h"
#include "Vehicle/HardPointTraceComponent.h"
#include "Vehicle/HullComponentCore.h"
#include "Vehicle/Vehicle.h"
#include "Vehicle/VehicleCore.h"

void USKM_ComponentCore::BeginPlay()
{
	Super::BeginPlay();
}

void USKM_ComponentCore::InitializeComponentCore(UHardPointTraceComponent* HardPointTrace)
{
	HardPointTraceComponent = HardPointTrace;
	HardPointTraceComponent->HardPointComponent = this;
}

void USKM_ComponentCore::ToggleComponentPreview(bool bShowPreview, USkeletalMesh* PreviewMesh)
{
	if(!bShowPreview) SetSkeletalMeshAsset(nullptr);
	else
	{
		SetSkeletalMeshAsset(PreviewMesh);
		
		// TODO: The Whole MID needs to be investigated
		 UMaterialInstanceDynamic* DynamicMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, PreviewMaterial);
		FSkeletalMaterial SkeletalMaterial = FSkeletalMaterial();
		SkeletalMaterial.MaterialInterface = DynamicMaterialInstance;
		SkeletalMaterial.MaterialSlotName = FName("Preview");
		//~

		if(IsValid(OwningHullMesh)) SetLeaderPoseComponent(OwningHullMesh);
	}
}

void USKM_ComponentCore::MountSkeletalComponent(bool bIsMounting, USkeletalMesh* ComponentMesh,
	TSubclassOf<UAnimInstance> ComponentAnimClass, FName RowName)
{
	if(!bIsMounting)
	{
		SetSkeletalMeshAsset(nullptr);
	}
	else
	{
		SetSkeletalMeshAsset(ComponentMesh);
		SetLeaderPoseComponent(nullptr);
		MountingSplash();
		if(ComponentAnimClass) SetAnimInstanceClass(ComponentAnimClass);
		ComponentAnimInstance = GetAnimInstance(); // TODO: Check if that's used
		// if(ComponentDataRow.DataTable.Get() && ComponentDataRow.RowName.IsValid())
		// {
		// 	if(const FComponentData* ComponentData =  ComponentDataRow.DataTable->FindRow<FComponentData>(ComponentDataRow.RowName, TEXT("")))
		// 	{
		// 		ComponentDataRow = 
		// 	}
		// }
		HandleSkeletalComponent();
	}
}

void USKM_ComponentCore::MountActorComponent(TSubclassOf<AActor> ComponentClass, FName HardPointSocket,
                                             FName DataRowName)
{
	// TODO: Investigate if this function could be simpler, without needing a SKM_ComponentCore
	SetSkeletalMeshAsset(nullptr); // TODO: Check if that's used
	SetLeaderPoseComponent(nullptr); // TODO: Check if that's used

	if(UWorld* World = GetWorld())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = OwningVehicle;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		const FTransform SpawnTransform = OwningHullMesh->GetSocketTransform(HardPointSocket);
		TargetComponentCore = Cast<AComponentCore>(World->SpawnActor(ComponentClass, &SpawnTransform, SpawnParameters));
		TargetComponentCore->OwningVehicle = OwningVehicle;
		TargetComponentCore->VehicleCore = OwningVehicle->GetVehicleCore();
		TargetComponentCore->ComponentDataRow = ComponentDataRow;

		const FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		TargetComponentCore->AttachToComponent(OwningHullMesh, TransformRules, HardPointSocket);
		MountingSplash();
		HandleActorComponent();

		// switch (HardPoint) {
		// case EHardPoint::EHP_None:
		// case EHardPoint::EHP_Lampost:
		// 	break;
		// case EHardPoint::EHP_Helm:
		// 	// SetupHelm()
		// 	break;
		// case EHardPoint::EHP_Engine:
		// 	SetupEngine(TargetComponentCore);
		// 	break;
		// case EHardPoint::EHP_Gauge:
		// 	// OwningVehicle->GetVehicleCore()->TryAddingFuelGauge(TargetComponentCore->);
		// 	SetupFuelGauge();
		// 	break;
		// default: ;
		// }

	}
}

AVehicle* USKM_ComponentCore::GetOwningVehicle_Implementation()
{
	return IComponentInterface::GetOwningVehicle_Implementation();
}

/*
 * Engine
 */
void USKM_ComponentCore::SetupEngine(AActor* ComponentActor)
{
	OwningVehicle->GetVehicleCore()->TryAddingEngine(TargetComponentCore);
}

/*
 * VFX
*/

void USKM_ComponentCore::MountingSplash() const
{
	if(IsValid(MountingSplashSystem))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,
		   MountingSplashSystem,
		   GetSmokeSocketTransform().GetLocation(),
		   GetSmokeSocketTransform().GetRotation().Rotator());
	}
}

FTransform USKM_ComponentCore::GetSmokeSocketTransform() const
{
	if(ComponentDataRow.DataTable.Get() && ComponentDataRow.RowName.IsValid())
	{
		if(const FComponentData* ComponentData =  ComponentDataRow.DataTable->FindRow<FComponentData>(ComponentDataRow.RowName, TEXT("")))
		{
			if (ComponentData->SmokeSocketName.IsNone())
			{
				return GetComponentTransform();
			}
			if(IsValid(OwningHullMesh))
			{
				return OwningHullMesh->GetSocketTransform(ComponentData->SmokeSocketName);
			}
		}
	}
	return FTransform();
}