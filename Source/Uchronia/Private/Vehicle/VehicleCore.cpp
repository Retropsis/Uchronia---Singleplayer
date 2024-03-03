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
	PrimaryComponentTick.bCanEverTick = false;
}

void UVehicleCore::BeginPlay()
{
	Super::BeginPlay();
	InitializeVehicleCore();
}

void UVehicleCore::InitializeVehicleCore()
{
	OwningVehicle = Cast<AVehicle>(GetOwner());
	if(const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(FuelConsumptionTimer, this, &UVehicleCore::UpdateFuelStatus, FuelConsumptionCooldown, true);
	}
	OnFuelChangeDelegate.AddDynamic(this, &UVehicleCore::UVehicleCore::UpdateFuelGauges);
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
}

void UVehicleCore::UpdateFuelStatus()
{
	if(!IsValid(FuelComponent)) return;
	
	bIsOutOfFuel = UKismetMathLibrary::NearlyEqual_FloatFloat(FuelComponent->GetCurrentFuelQuantity(), 0.f, 0.01);
	OnFuelChangeDelegate.Broadcast();
	if(bIsOutOfFuel != bLastIsOutOfFuel)
	{
		OnFuelEmptyDelegate.Broadcast(bIsOutOfFuel ? 0.f : 1.f);
		bLastIsOutOfFuel = bIsOutOfFuel;
	}
}

bool UVehicleCore::TickFuelConsumption(const float QueriedAmount)
{
	const bool bRetrievedWasNearZero = UKismetMathLibrary::NearlyEqual_FloatFloat(FuelComponent->RetrieveFuel(QueriedAmount), 0.f, 0.01f);
	UpdateFuelStatus();
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

