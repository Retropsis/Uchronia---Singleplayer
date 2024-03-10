// Retropsis @ 2023-2024

#include "Vehicle/GroundVehicleMovement.h"
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
	EMovementState State = VehicleCore->GetMovementState();
	GEngine->AddOnScreenDebugMessage(46, 1.f, FColor::Green, FString::Printf(TEXT("State: %s"), *UEnum::GetValueAsString(State)));
	
	switch (VehicleCore->GetMovementState()) {
	case EMovementState::EMS_None:
		TargetForwardSpeed = 0.f;
		TargetTurnSpeed = 0.f;
		break;
	case EMovementState::EMS_Engine_Off_Unable:
		TargetForwardSpeed = 0.f;
		TargetTurnSpeed = 0.f;
		break;
	case EMovementState::EMS_Engine_Off_Ready:
		TargetForwardSpeed = 0.f;
		TargetTurnSpeed = 0.f;
		break;
	case EMovementState::EMS_Engine_On_Idling:
		break;
	case EMovementState::EMS_Engine_On_Moving:
		break;
	case EMovementState::EMS_Engine_On_Easing_In:
		break;
	case EMovementState::EMS_Engine_On_Easing_Out:
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
		const FRotator Rotation4 = UKismetMathLibrary::RInterpTo(OwningVehicle->GetActorRotation(), Rotation3, DeltaTime, VehicleRotationInterpSpeed);
		OwningVehicle->SetActorRotation(Rotation4);

		/*
		 *
		 */
		FVector X = OwningVehicle->GetActorForwardVector() *
			UKismetMathLibrary::FInterpTo(CurrentForwardSpeed, TargetForwardSpeed, DeltaTime, ForwardInterpSpeed) *  DeltaTime;
		FVector Y = OwningVehicle->GetActorUpVector() * DeltaZ;
		OwningVehicle->AddActorWorldOffset(X + Y, true);
		CurrentForwardSpeed = UKismetMathLibrary::FInterpTo(CurrentForwardSpeed, TargetForwardSpeed, DeltaTime, ForwardInterpSpeed);

		/*
		 *
		 */
		CurrentHandleAngle = FMath::Clamp(UKismetMathLibrary::FInterpTo(CurrentHandleAngle, TargetHandleAngle, DeltaTime, HandleInterpSpeed), -35.f, 35.f);
		
	}
	if(VehicleCore->GetMovementState() == EMovementState::EMS_Engine_On_Easing_Out &&
		UKismetMathLibrary::NearlyEqual_FloatFloat(CurrentForwardSpeed, 0.f, 0.01f))
	{
		VehicleCore->SetMovementState(EMovementState::EMS_Engine_Off_Ready);
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
		TargetForwardSpeed = EngineCount * 70.f;
		break;
	case EGears::EST_2:
		TargetForwardSpeed = EngineCount * 140.f;
		break;
	case EGears::EST_3:
		TargetForwardSpeed = EngineCount * 210.f;
		break;
	case EGears::EST_4:
		TargetForwardSpeed = EngineCount * 210.f;
		break;
	case EGears::EST_5:
		TargetForwardSpeed = EngineCount * 210.f;
		break;
	case EGears::EST_6:
		TargetForwardSpeed = EngineCount * 210.f;
		break;
	case EGears::EG_MAX:
		break;
	default: ;
	}
}
