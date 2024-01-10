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
