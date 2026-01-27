#include "GAS_Crash/Public/Character/MyBaseCharacter.h"
#include "AbilitySystemComponent.h"

AMyBaseCharacter::AMyBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

UAbilitySystemComponent* AMyBaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

void AMyBaseCharacter::GiveStartupAbilities()
{
	if (!IsValid(GetAbilitySystemComponent())) return;
	for (const auto& Ability : StartupGameplayAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
		GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	}
}

void AMyBaseCharacter::InitializeAttribute() const
{
	//check
	if (!IsValid(GetAbilitySystemComponent()))
	{
		UE_LOG(LogTemp, Error, TEXT("ASC is invalid in InitializeAttribute"));
		return;
	}
	
	if (!IsValid(InitializeAttributesEffects))
	{
		UE_LOG(LogTemp,Error,TEXT("InitializeAttributesEffects not set for %s"),*GetName());
		return;
	}	
	
	//Create EffectContextHandle 
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this); //mark,this is created from self
	
	//Create EffectSpecHandle
	FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
		InitializeAttributesEffects,
		1.f,
		EffectContextHandle);
	
	
	if (!EffectSpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create EffectSpec for %s"), *GetName()); 
		return;
	}
	
	//Apply Effect to Character's Attribute
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

