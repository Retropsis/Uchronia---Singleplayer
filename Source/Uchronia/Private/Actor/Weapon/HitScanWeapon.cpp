// Retropsis @ 2023-2024

#include "Actor/Weapon/HitScanWeapon.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

void AHitScanWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if(WeaponDataRow.DataTable.Get() && WeaponDataRow.RowName.IsValid())
	{
		if(const FWeaponData* WeaponData =  WeaponDataRow.DataTable->FindRow<FWeaponData>(WeaponDataRow.RowName, TEXT("")))
		{
			bUseScatter = WeaponData->bUseScatter;
			DistanceToSphere = WeaponData->WeaponStatistics.DistanceToSphere;
			SphereRadius = WeaponData->WeaponStatistics.SphereRadius;
		}
	}
}

void AHitScanWeapon::Trigger(const FVector& HitTarget)
{
	Super::Trigger(HitTarget);

	if (const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash")))
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();
		
		FHitResult ScanHit;
		WeaponTraceHit(Start, HitTarget, ScanHit);
		CauseDamage(ScanHit);
		ApplyWeaponEffects(SocketTransform, ScanHit);
	}
}

void AHitScanWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutTraceHit) const
{
	// TODO: Need a valid HitTarget when hitting the sky ?
	if(const UWorld* World = GetWorld())
	{
		const FVector End = bUseScatter ? TraceEndWithScatter(TraceStart, HitTarget) : TraceStart + (HitTarget - TraceStart)  * 1.25f;
		World->LineTraceSingleByChannel(OutTraceHit, TraceStart, End, ECC_Visibility);
		FVector BeamEnd = End;
		if(OutTraceHit.bBlockingHit)
		{
			BeamEnd = OutTraceHit.ImpactPoint;
		}
		if (BeamParticles)
		{
			if(UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(World, BeamParticles, TraceStart, FRotator::ZeroRotator, true))
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
		if (BeamLaser)
		{
			if(UNiagaraComponent* Beam = UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, BeamLaser, TraceStart, FRotator::ZeroRotator))
			{
				Beam->SetVariableVec3(FName("Beam End"), BeamEnd);
			}
		}
	}
}

// TODO: DistanceToSphere, SphereRadius and CrosshairSpread should be related to the Weapon Properties 
FVector AHitScanWeapon::TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget) const
{
	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	const FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;
	const FVector RandomVector = UKismetMathLibrary::RandomUnitVector() * FMath::RandRange(0.f, SphereRadius);
	const FVector EndLocation = SphereCenter + RandomVector;
	const FVector ToEndLocation = EndLocation - TraceStart;

	if(bVisualizeScatter)
	{
		DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 12, FColor::Red, false, 5.f);
        DrawDebugSphere(GetWorld(), EndLocation, 4.f, 12, FColor::White, false, 5.f);
        DrawDebugLine(GetWorld(), TraceStart, FVector(TraceStart + ToEndLocation * TRACE_LENGTH / ToEndLocation.Size()), FColor::Cyan, false, 3.f);
	}
	
	return FVector(TraceStart + ToEndLocation * TRACE_LENGTH / ToEndLocation.Size());
}

void AHitScanWeapon::ApplyWeaponEffects(const FTransform& SocketTransform, const FHitResult& Hit) const
{
	UWorld* World = GetWorld();
	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticles, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
	}
	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(World, ImpactSound, Hit.ImpactPoint);
	}
	if(MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(World, MuzzleFlash, SocketTransform);
	}
	if(FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(World, FireSound, SocketTransform.GetLocation());
	}
}
