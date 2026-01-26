#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "MyBaseCharacter.generated.h"

class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UAbilitySystemComponent;

// Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASCInitalized,UAbilitySystemComponent*,ASC,UAttributeSet*,AS);

//Abstract Mark : cannot be instantiated; intended for inheritance only;
UCLASS(Abstract)
class GAS_CRASH_API AMyBaseCharacter : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMyBaseCharacter();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual UAttributeSet* GetAttributeSet() const{ return nullptr;}
	
	//定义 初始化ASC委托,用于异步回调 初始化
	UPROPERTY(BlueprintAssignable)
	FASCInitalized OnAscInitialized;
protected:
	void GiveStartupAbilities();
	
	//Apply Effects to CharacterAttribute
	void InitializeAttribute() const;
private:
	UPROPERTY(EditDefaultsOnly,Category="GC|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupGameplayAbilities;
	
	UPROPERTY(EditDefaultsOnly,Category="GC|Effects")
	TSubclassOf<UGameplayEffect> InitializeAttributesEffects;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
