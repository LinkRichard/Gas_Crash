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
	
	//Attribute初始化通知委托,作用:通知WidgetComponent,AttributeSet通过GE,初始化完成.
	UPROPERTY(BlueprintAssignable)
	FAttributeSetInitialized OnAttributeSetInitialized;
	
	//初始化标记,用来告诉WidgetComponent是否初始化属性完成. 此处为什么需要复制,客户端需要知道服务器是否初始化完成,避免过早绑定.
	UPROPERTY(ReplicatedUsing = OnRep_AttributeInitialized)
	bool bAttributeInitialized = false;
	
	//委托广播,在客户端和服务器是独立的,此函数在客户端通知AS初始化完成
	UFUNCTION()
	void OnRep_AttributeInitialized();
	
	//属性修改后置回调 (仅在数值变动生效后触发),触发时机：GE 修改 BaseValue 之后,属性同步之前,Server必定触发,Client仅在本地预测成功时触发
	virtual void  PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	
};
