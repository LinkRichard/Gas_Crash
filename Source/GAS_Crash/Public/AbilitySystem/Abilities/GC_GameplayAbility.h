#pragma once
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GC_GameplayAbility.generated.h"

class AGC_EnemyCharacter;
class AMyBaseCharacter;
class UAbilitySystemComponent;

UCLASS()
class GAS_CRASH_API UGC_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="GC|Debug")
	bool bDrawDebugs = false;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//通过 ActorInfo 获取 GC_ASC，避免蓝图重复 Cast
	UFUNCTION(BlueprintCallable, Category="GC|Ability")
	UAbilitySystemComponent* GetGCAbilitySystemComponent() const;

	//获取 BaseCharacter，通过 AvatarActor（物理表现层）
	UFUNCTION(BlueprintCallable, Category="GC|Ability")
	AMyBaseCharacter* GetBaseCharacter() const;

	//获取 EnemyCharacter,通过 AvatarActor
	UFUNCTION(BlueprintCallable, Category="GC|Ability")
	AGC_EnemyCharacter* GetGC_EnemyCharacter() const;
};
