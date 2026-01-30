#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "MyBaseCharacter.generated.h"

struct FOnAttributeChangeData;
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
	
	bool IsAlive() const {return bAlive;}
	
	void SetAlive(bool AliveStatus) {bAlive = AliveStatus;}
	
	//Listen Health Changed
	void OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData);
	
	//Handle Death
	virtual void HandleDeath();
	
	//Handle Respawn
	UFUNCTION(BlueprintCallable,Category="GC|Death")
	virtual void HandleRespawn();
	
	//定义 初始化ASC委托,用于异步回调 初始化
	UPROPERTY(BlueprintAssignable)
	FASCInitalized OnAscInitialized;
protected:
	void GiveStartupAbilities();
	
	//Apply Effects to CharacterAttribute
	void InitializeAttribute() const;
	
	//Apply ResetAttributeEffect to CharacterAttribute
	UFUNCTION(BlueprintCallable,Category="GC|Attributes")
	void ResetAttributes();
private:
	UPROPERTY(EditDefaultsOnly,Category="GC|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupGameplayAbilities;
	
	UPROPERTY(EditDefaultsOnly,Category="GC|Effects")
	TSubclassOf<UGameplayEffect> InitializeAttributesEffects;
	
	UPROPERTY(EditDefaultsOnly,Category="GC|Effects")
	TSubclassOf<UGameplayEffect> ResetAttributeEffects;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly,meta=(AllowPrivateAccess = "true"))
	bool bAlive = true;
};
