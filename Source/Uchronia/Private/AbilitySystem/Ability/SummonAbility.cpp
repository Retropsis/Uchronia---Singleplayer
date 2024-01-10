// Retropsis @ 2023-2024

#include "AbilitySystem/Ability/SummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> USummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / (NumMinions + 1);
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis( - SpawnSpread / 2.f, FVector::UpVector);
	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + LeftOfSpread * MaxSpawnDistance, 1.f, FLinearColor::White, 3.f);
	// DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MinSpawnDistance, 15.f, 12, FColor::White, false, 3.f);
	// DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MaxSpawnDistance, 15.f, 12, FColor::White, false, 3.f);
	
	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);
	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + RightOfSpread * MaxSpawnDistance, 1.f, FLinearColor::White, 3.f);
	// DrawDebugSphere(GetWorld(), Location + RightOfSpread * MinSpawnDistance, 15.f, 12, FColor::White, false, 3.f);
	// DrawDebugSphere(GetWorld(), Location + RightOfSpread * MaxSpawnDistance, 15.f, 12, FColor::White, false, 3.f);

	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * (i + 1), FVector::UpVector);
		FVector SelectedSpawnLocation = Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, SelectedSpawnLocation + FVector(0.f, 0.f, 400.f), SelectedSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility);
		if(Hit.bBlockingHit)
		{
			SelectedSpawnLocation = Hit.ImpactPoint;
		}
		SpawnLocations.Add(SelectedSpawnLocation);
		
		// DrawDebugSphere(GetWorld(), SelectedSpawnLocation, 20.f, 12, FColor::Red, false, 3.f);
		// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + Direction * MaxSpawnDistance, 1.f, FLinearColor::Green, 3.f);
		// DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 10.f, 12, FColor::Green, false, 3.f);
		// DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 10.f, 12, FColor::Green, false, 3.f);
	}
	return SpawnLocations;
}

TSubclassOf<APawn> USummonAbility::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}
