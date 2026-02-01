#include "AbilitySystem/Abilities/GC_GameplayAbility.h"
#include "Character/MyBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "Character/GC_EnemyCharacter.h"

void UGC_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (bDrawDebugs && IsValid(GEngine))
	{
		GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Cyan,FString::Printf(TEXT("%s Activated"), *GetName()));
	}
}

UAbilitySystemComponent* UGC_GameplayAbility::GetGCAbilitySystemComponent() const
{
	// ActorInfo 在 Ability 激活时由系统注入，保证线程安全和网络同步正确
	// const_cast 是安全的，因为 ActorInfo 本身就是可变的（ASC 是 TWeakObjectPtr）
	const FGameplayAbilityActorInfo& Info = GetActorInfo();
	return Info.AbilitySystemComponent.Get();
}

AMyBaseCharacter* UGC_GameplayAbility::GetBaseCharacter() const
{
	// AvatarActor = 物理表现（如 Character），OwnerActor = 逻辑拥有者（如 PlayerController）
	// 大多数情况下二者指向同一 Character
	return Cast<AMyBaseCharacter>(GetAvatarActorFromActorInfo());
}

AGC_EnemyCharacter* UGC_GameplayAbility::GetGC_EnemyCharacter() const
{
	return Cast<AGC_EnemyCharacter>(GetAvatarActorFromActorInfo());
}
