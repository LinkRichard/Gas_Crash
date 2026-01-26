#pragma once
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GC_AttributeSet.generated.h"

// use macro auto spawn code
#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) 

//Create Delegate 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttributeSetInitialized);

UCLASS()
class GAS_CRASH_API UGC_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FAttributeSetInitialized OnAttributeSetInitialized;
	
	
	//================Core Attribute=================
	ATTRIBUTE_ACCESSORS(ThisClass,Health);
	ATTRIBUTE_ACCESSORS(ThisClass,MaxHealth);
	ATTRIBUTE_ACCESSORS(ThisClass,Mana);
	ATTRIBUTE_ACCESSORS(ThisClass,MaxMana);
	
	//================Attribute Define================
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Mana)
	FGameplayAttributeData Mana;
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	
	
	
	//================Hook Function========================
	
	// Replicate Server changed Health to client
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue) const;
	
	
	// 复制注册机制,向ue的网络系统注册需要复制的值
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	UPROPERTY(ReplicatedUsing = OnRep_AttributeInitialized)
	bool bAttributeInitialized = false;
	
	UFUNCTION()
	void OnRep_AttributeInitialized();
	
	//
	virtual void  PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	
};
