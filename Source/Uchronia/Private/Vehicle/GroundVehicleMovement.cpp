// Retropsis @ 2023-2024

#include "Vehicle/GroundVehicleMovement.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Vehicle/GroundSensingComponent.h"
#include "Vehicle/HullComponentCore.h"
#include "Vehicle/Vehicle.h"
#include "Vehicle/VehicleCore.h"

void UGroundVehicleMovement::InitializeGroundVehicleMovement(UVehicleCore* InVehicleCore)
{
	OwningVehicle = Cast<AVehicle>(GetOwner());
	if(IsValid(OwningVehicle))
	{
		OwningVehicle->OnGearChangeDelegate.AddDynamic(this, &UGroundVehicleMovement::SetThrustSpeedByGear);
	}
	
	VehicleCore = InVehicleCore;	
	HullReference = Cast<UHullComponentCore>(GetOwner()->GetRootComponent());
}

void UGroundVehicleMovement::SolveMovement(float DeltaTime)
{
	switch (VehicleCore->GetMovementState()) {
	case EMovementState::EMS_None:
		TargetForwardSpeed = 0.f;
		TargetTurnSpeed = 0.f;
		break;
	case EMovementState::EMS_Engine_Off:
		TargetForwardSpeed = 0.f;
		TargetTurnSpeed = 0.f;
		break;
	case EMovementState::EMS_Idle:
		break;
	case EMovementState::EMS_Moving:
		break;
	case EMovementState::EMS_Ease_In:
		break;
	case EMovementState::EMS_Ease_Out:
		TargetForwardSpeed = 0.f;
		TargetTurnSpeed = 0.f;
		break;
	default: ;
	}
	
	AverageNormal = FVector::ZeroVector;
	AverageHeight = 0.f;
	CountOnGround = 0;

	for (const UGroundSensingComponent* Sensor : GroundSensors)
	{
		if(Sensor->bOnGround)
		{
			AverageNormal = Sensor->HitNormal + AverageNormal;
			AverageHeight = Sensor->HitLocation.Z + AverageHeight;
			CountOnGround++;
		}
		
		/*
		 *
		 */
		AverageNormal = AverageNormal / CountOnGround;
		AverageHeight = AverageHeight / CountOnGround;
		DeltaZ = AverageHeight + HeightOffset - OwningVehicle->GetActorLocation().Z;
		const FRotator Rotation = UKismetMathLibrary::MakeRotator(0.f, 0.f, TargetTurnSpeed * DeltaTime);
		const FRotator Rotation2 = UKismetMathLibrary::MakeRotFromZY(AverageNormal, OwningVehicle->GetActorRightVector());
		const FRotator Rotation3 = UKismetMathLibrary::ComposeRotators(Rotation, Rotation2);
		const FRotator Rotation4 = UKismetMathLibrary::RInterpTo(OwningVehicle->GetActorRotation(), Rotation3, DeltaTime, 8.f);
		OwningVehicle->SetActorRotation(Rotation4);

		/*
		 *
		 */
		FVector X = OwningVehicle->GetActorForwardVector() *
			UKismetMathLibrary::FInterpTo(CurrentForwardSpeed, TargetForwardSpeed, DeltaTime, 8.f) *  DeltaTime;
		FVector Y = OwningVehicle->GetActorUpVector() * DeltaZ;
		OwningVehicle->AddActorWorldOffset(X + Y, true);
		CurrentForwardSpeed = UKismetMathLibrary::FInterpTo(CurrentForwardSpeed, TargetForwardSpeed, DeltaTime, 8.f);

		/*
		 *
		 */
		CurrentHandleAngle = FMath::Clamp(UKismetMathLibrary::FInterpTo(CurrentHandleAngle, TargetHandleAngle, DeltaTime, 8.f), -35.f, 35.f);
		
	}
}

void UGroundVehicleMovement::SetThrustSpeedByGear(EGears InCurrentGear)
{
	const int32 EngineCount = VehicleCore->GetEngineData();
	switch (InCurrentGear)
	{
	case EGears::EST_R:
		TargetForwardSpeed = EngineCount * -50.f;
		break;
	case EGears::EST_N:
		TargetForwardSpeed = 0.f;
		break;
	case EGears::EST_1:
		TargetForwardSpeed = EngineCount * 100.f;
		break;
	case EGears::EST_2:
		TargetForwardSpeed = EngineCount * 200.f;
		break;
	case EGears::EST_3:
		TargetForwardSpeed = EngineCount * 320.f;
		break;
	case EGears::EST_4:
		TargetForwardSpeed = EngineCount * 320.f;
		break;
	case EGears::EST_5:
		TargetForwardSpeed = EngineCount * 320.f;
		break;
	case EGears::EST_6:
		TargetForwardSpeed = EngineCount * 320.f;
		break;
	case EGears::EG_MAX:
		break;
	default: ;
	}
}
