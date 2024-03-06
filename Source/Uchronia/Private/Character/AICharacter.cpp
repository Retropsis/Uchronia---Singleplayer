// Retropsis @ 2023

#include "Character/AICharacter.h"
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "UchroniaBlueprintFunctionLibrary.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AI/BaseAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Uchronia/Uchronia.h"
#include "UI/Widget/BaseUserwidget.h"

AAICharacter::AAICharacter()
{
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Movement Tweaks
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if(!HasAuthority()) return;
	
	BaseAIController = Cast<ABaseAIController>(NewController);
	BaseAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	BaseAIController->RunBehaviorTree(BehaviorTree);
	BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);

	// TODO: Define which classes are Ranged or have some way to check it
	BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass == ECharacterClass::Soldier || CharacterClass == ECharacterClass::Leviathan);
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	StartLocation = GetActorLocation();
	InitAbilityActorInfo();
	
	if(HasAuthority())
	{
		UUchroniaBlueprintFunctionLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}

	if(UBaseUserWidget* BaseUserWidget =  Cast<UBaseUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		BaseUserWidget->SetWidgetController(this);
	}

	if(const UBaseAttributeSet* BaseAS = CastChecked<UBaseAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAS->GetHealthAttribute()).AddLambda(
			[this] (const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAS->GetMaxHealthAttribute()).AddLambda(
			[this] (const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
		AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTags.Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAICharacter::HitReactTagChanged);
		OnHealthChanged.Broadcast(BaseAS->GetHealth());
		OnMaxHealthChanged.Broadcast(BaseAS->GetMaxHealth());
	}
}

void AAICharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if(HasAuthority())
	{
		InitializeDefaultAttributes();		
	}
}

void AAICharacter::InitializeDefaultAttributes() const
{
	UUchroniaBlueprintFunctionLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);	
}

// Called through CombatInterface
void AAICharacter::HitReact(const FVector& ImpactPoint)
{
}

void AAICharacter::Die()
{
	SetLifeSpan(DespawnLifeSpan);
	if(BaseAIController) BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	Super::Die();
}

// Called through Tag Event
void AAICharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	// if(!HasAuthority()) return;
	if(BaseAIController && BaseAIController->GetBlackboardComponent())
	{
		BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

int32 AAICharacter::GetCharacterLevel()
{
	return Level;
}

/*
 * Enemy Interface
 */
void AAICharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AAICharacter::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

FVector AAICharacter::FindRandomLocation_Implementation()
{
	int32 i = 0;
	while (i < 10)
	{
		i++;
		const float RandomX = GetActorLocation().X + FMath::FRandRange(-1000.f, 1000.f);
		const float RandomY = GetActorLocation().Y + FMath::FRandRange(-1000.f, 1000.f);
		TargetLocation = FVector(RandomX, RandomY, GetActorLocation().Z);
		if((StartLocation - TargetLocation).Size() > RoamRadius)
		{
			TargetLocation = StartLocation;
		}
		else
		{
			return TargetLocation;
		}
	}
	return TargetLocation;
	
	// if((StartLocation - TargetLocation).Size() > RoamRadius)
	// {
	// 	i++;
	// 	if(i < 10)
	// 	{
	// 		TargetLocation = FVector(RandomX, RandomY, GetActorLocation().Z);
	// 	}
	// 	else
	// 	{
	// 		return StartLocation;
	// 	}
	// }
	// else
	// {
	// 	return TargetLocation;
	// }
}

bool AAICharacter::MoveToLocation_Implementation(FVector ToLocation, float Threshold)
{
	if ((GetActorLocation() - ToLocation).Size() > Threshold)
	{
		const FVector Target = FVector(ToLocation.X, ToLocation.Y, GetActorLocation().Z);
		const FRotator LookAtRotation= UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
		const FVector Direction = UKismetMathLibrary::Conv_RotatorToVector(FRotator(0.f, LookAtRotation.Yaw, 0.f));
		AddMovementInput(Direction, 1, true);
		
		const FRotator Rotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
		const FRotator NewRot = UKismetMathLibrary::RLerp(GetActorRotation(), Rotation, 0.1f, true);
		SetActorRotation(NewRot);
		return false;
	}
	return true;
}

bool AAICharacter::ChasePlayer_Implementation()
{
	switch (EnemyState) {
		return false;
	case EEnemyStates::EES_None:
		break;
	case EEnemyStates::EES_Idle:
		break;
	case EEnemyStates::EES_Patrol:
		break;
	case EEnemyStates::EES_Chase:
		if (IsValid(CombatTarget) && GetDistanceTo(CombatTarget) > AttackDistance)
		{
			const FVector LookAtLocation = FVector(CombatTarget->GetActorLocation().X, CombatTarget->GetActorLocation().Y, GetActorLocation().Z);
			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LookAtLocation);
			const FVector Direction = UKismetMathLibrary::Conv_RotatorToVector(FRotator(0.f, LookAtRotation.Yaw, 0.f));
			AddMovementInput(Direction);
			
			const FRotator Rotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
			const FRotator NewRot = UKismetMathLibrary::RLerp(GetActorRotation(), Rotation, 0.1f, true);
			SetActorRotation(NewRot);
		}
		else if (IsValid(CombatTarget) && GetDistanceTo(CombatTarget) < AttackDistance - 100.f)
		{
			const FVector LookAtLocation = FVector(CombatTarget->GetActorLocation().X, CombatTarget->GetActorLocation().Y, GetActorLocation().Z);
			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LookAtLocation);
			const FVector Direction = UKismetMathLibrary::Conv_RotatorToVector(FRotator(0.f, LookAtRotation.Yaw, 0.f));
			AddMovementInput(Direction, -1.f);
			
			const FRotator Rotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
			const FRotator NewRot = UKismetMathLibrary::RLerp(GetActorRotation(), Rotation, 0.1f, true);
			SetActorRotation(NewRot);
		}
		break;
	case EEnemyStates::EES_Fall:
		break;
	case EEnemyStates::EES_Investigate:
		break;
	case EEnemyStates::EES_MAX:
		break;
	default: ;
	}
	return false;
}