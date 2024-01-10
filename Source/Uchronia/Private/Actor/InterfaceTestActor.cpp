// Retropsis @ 2023-2024


#include "Actor/InterfaceTestActor.h"

AInterfaceTestActor::AInterfaceTestActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
}

void AInterfaceTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;
}

void AInterfaceTestActor::BeginFocus()
{
	if(Mesh)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, FString::Printf(TEXT("BeginFocus")));
		// Mesh->SetRenderCustomDepth(true);
	}
}

void AInterfaceTestActor::EndFocus()
{
	if(Mesh)
	{
		GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Red, FString::Printf(TEXT("EndFocus")));
		// Mesh->SetRenderCustomDepth(false);
	}
}

void AInterfaceTestActor::BeginInteract()
{
		GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Orange, FString::Printf(TEXT("BeginInteract")));
}

void AInterfaceTestActor::EndInteract()
{
		GEngine->AddOnScreenDebugMessage(4, 3.f, FColor::Orange, FString::Printf(TEXT("EndInteract")));
}

void AInterfaceTestActor::Interact(APlayerCharacter* PlayerCharacter)
{
		GEngine->AddOnScreenDebugMessage(5, 3.f, FColor::Green, FString::Printf(TEXT("Interact")));
}

