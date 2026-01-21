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
	
	
	// 6. 验证逻辑优化
	if (!ActiveGameplayEffectHandle.IsValid())
	{
		// 如果是 Instant GE，这行 Log 会被误触发
		// 建议：只有当 Duration 不是 Instant 时才进行这个检查
		if (EffectSpecHandle.Data.Get()->Def->DurationPolicy != EGameplayEffectDurationType::Instant)
		{
			UE_LOG(LogTemp, Error, TEXT("Active GE failed to apply (Duration/Infinite) to %s"), *GetName());
		}
		else 
		{
			// Instant 效果应用成功通常没有返回值，这是正常的
			UE_LOG(LogTemp, Log, TEXT("Instant GE applied to %s"), *GetName());
		}
	}
}

