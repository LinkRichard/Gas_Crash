#pragma once
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GC_AbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_CRASH_API UGC_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual void OnRep_ActivateAbilities() override;
	
	//Set the GameAbility Level
	UFUNCTION(BlueprintCallable,Category="GC|Abilities")
	void SetAbilityLevel(TSubclassOf<UGameplayAbility> AbilityClass,int32 Level);
	
	//Increase Ability Level,default increase 1 level.
	UFUNCTION(BlueprintCallable,Category="GC|Abilities")
	void IncreaseAbilityLevel(TSubclassOf<UGameplayAbility> AbilityClass,int32 IncreaseLevel = 1);
	
private:
	void HandleAutoActivatedAbilities(const FGameplayAbilitySpec& AbilitySpec);
};
