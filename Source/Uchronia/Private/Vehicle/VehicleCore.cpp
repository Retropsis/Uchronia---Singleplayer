// Retropsis @ 2023-2024

#include "Vehicle/VehicleCore.h"
#include "NiagaraFunctionLibrary.h"
#include "Interaction/EngineInterface.h"
#include "Interaction/FuelInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Vehicle/ComponentCore.h"
#include "Vehicle/FuelComponent.h"
#include "Vehicle/HullComponentCore.h"
#include "Vehicle/SKM_ComponentCore.h"
#include "Vehicle/Vehicle.h"

UVehicleCore::UVehicleCore()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVehicleCore::BeginPlay()
{
	Super::BeginPlay();
}

void UVehicleCore::InitializeVehicleCore()
{
	OwningVehicle = Cast<AVehicle>(GetOwner());
	if(IsValid(OwningVehicle))
	{
		OwningVehicle->OnGearChangeDelegate.AddDynamic(this, &UVehicleCore::HandleOnGearChanged);
	}
	
	if(const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(FuelConsumptionTimer, this, &UVehicleCore::UpdateFuelStatus, FuelConsumptionCooldown, true);
	}
	OnFuelChangeDelegate.AddDynamic(this, &UVehicleCore::UpdateFuelGauges);
	OnFuelEmptyDelegate.AddDynamic(this, &UVehicleCore::HandleOnFuelEmptied);
	TArray<FName> SocketNames = OwningVehicle->GetHullMesh()->GetAllSocketNames();
	for (FName SocketName : SocketNames)
	{
		if (SocketName.ToString().Contains("SplashLoc"))
		{
			WaterSplashesSockets.Add(SocketName);
		}
	}
}

void UVehicleCore::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ToggleWaterSplashes();
}

void UVehicleCore::UpdateFuelStatus()
{
	if(!IsValid(FuelComponent)) return;
	
	bIsOutOfFuel = UKismetMathLibrary::NearlyEqual_FloatFloat(FuelComponent->GetCurrentFuelQuantity(), 0.f, 0.01);
	
	if(ShouldTickFuelConsumption()) // False is Retrieved was Near Zero
	{
		ToggleEngines(true);
	}
	else
	{
		ToggleEngines(false);
	}
	
	if (bIsOutOfFuel /*!ShouldTickFuelConsumption()*/)
	{
		if(bIsOutOfFuel != bLastIsOutOfFuel)
		{
			OnFuelEmptyDelegate.Broadcast(bIsOutOfFuel ? 0.f : 1.f);
			bLastIsOutOfFuel = bIsOutOfFuel;
		}
		ToggleEngines(false);
	}
	else
	{
		
	}
	OnFuelChangeDelegate.Broadcast();
}

bool UVehicleCore::ShouldTickFuelConsumption()
{
	return TickFuelConsumption(CurrentFuelTickConsumption);
}

bool UVehicleCore::TickFuelConsumption(const float QueriedAmount)
{
	const bool bRetrievedWasNearZero = UKismetMathLibrary::NearlyEqual_FloatFloat(FuelComponent->RetrieveFuel(QueriedAmount), 0.f, 0.01f);
	return !bRetrievedWasNearZero;
}

void UVehicleCore::UpdateFuelGauges()
{
	for (const TObjectPtr<USKM_ComponentCore> Gauge : FuelGauges)
	{
		if(Gauge->GetAnimInstance()->Implements<UFuelInterface>())
		{
			IFuelInterface::Execute_UpdateFuelGauge(Gauge->GetAnimInstance(), FuelComponent->GetCurrentFuelQuantity());
		}
	}
}

void UVehicleCore::ToggleEngines(const bool ShouldActivate)
{
	for (TObjectPtr<AComponentCore> Engine : Engines)
	{
		if(Engine->Implements<UEngineInterface>())
		{
			IEngineInterface::Execute_UpdateEngineTrail(Engine, ShouldActivate);
		}
	}
}

// TODO: Use DataTable to determine each engine power and fuel efficiency
void UVehicleCore::SetFuelTickConsumptionByGear(const EGears NewGear)
{
	switch (NewGear)
	{
	case EGears::EST_R:
		CurrentFuelTickConsumption = EngineCount * 0.01f;
		break;
	case EGears::EST_N:
		CurrentFuelTickConsumption = 0.f;
		break;
	case EGears::EST_1:
		CurrentFuelTickConsumption = EngineCount * 0.02f;
		break;
	case EGears::EST_2:
		CurrentFuelTickConsumption = EngineCount * 0.05f;
		break;
	case EGears::EST_3:
		CurrentFuelTickConsumption = EngineCount * 0.1f;
		break;
	case EGears::EST_4:
		CurrentFuelTickConsumption = EngineCount * 0.1f;
		break;
	case EGears::EST_5:
		CurrentFuelTickConsumption = EngineCount * 0.1f;
		break;
	case EGears::EST_6:
		CurrentFuelTickConsumption = EngineCount * 0.1f;
		break;
	case EGears::EG_MAX:
		break;
	default: ;
	}
}

void UVehicleCore::HandleOnGearChanged(EGears NewGear)
{
	SetFuelTickConsumptionByGear(NewGear);
	SolveMovementState(NewGear);
}

void UVehicleCore::SolveMovementState(EGears NewGear)
{
	if(MovementState == EMovementState::EMS_Ease_Out) return;
	if(NewGear != EGears::EST_N) MovementState = EMovementState::EMS_Moving;
}

void UVehicleCore::HandleOnFuelEmptied(float InFuelModifier)
{
	MovementState = EMovementState::EMS_Ease_Out;
	FuelModifier = FMath::Clamp(InFuelModifier, 0.f, 1.f);
}

/*
 * VFX
 */
void UVehicleCore::ToggleWaterSplashes()
{
	if(!IsValid(OwningVehicle)) return;
	
	if(OwningVehicle->GetActorLocation().Z < WaterSplashThreshold && !bEnableWaterSplash)
	{
		if(GetWorld()) GetWorld()->GetTimerManager().SetTimer(WaterSplashTimer, this, &UVehicleCore::WaterSplashAtLocation, WaterSplashCooldown, true);
		bEnableWaterSplash = true;
	}
	if(OwningVehicle->GetActorLocation().Z > WaterSplashThreshold)
	{
		if(GetWorld()) GetWorld()->GetTimerManager().ClearTimer(WaterSplashTimer);
		bEnableWaterSplash = false;
	}
}

void UVehicleCore::WaterSplashAtLocation()
{
	if(!IsValid(OwningVehicle) && !IsValid(OwningVehicle->GetHullMesh())) return;
	
	for (const FName Socket : WaterSplashesSockets)
	{
		FTransform SocketTransform = OwningVehicle->GetHullMesh()->GetSocketTransform(Socket);
		const FVector Location = FVector(SocketTransform.GetLocation().X, SocketTransform.GetLocation().Y, 0.f);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, WaterSplash, Location, SocketTransform.Rotator());
	}
}

