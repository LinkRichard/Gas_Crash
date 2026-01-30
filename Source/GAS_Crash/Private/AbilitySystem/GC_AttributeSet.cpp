#include "AbilitySystem/GC_AttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameplayTags/GCTags.h"
#include "Net/UnrealNetwork.h"

void UGC_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	//时机: 仅在客户端触发,当服务器复制到客户端的值,也就是下面参数Health,与本地预测的值OldValue不一样时触发
	//作用: 将新值注入ASC,对齐基准值,处理预测(回滚),触发所有订阅了Health变化的委托.
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass,Health,OldValue);
}

void UGC_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass,MaxHealth,OldValue);
}

void UGC_AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass,Mana,OldValue);
}

void UGC_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass,MaxMana,OldValue);
}



void UGC_AttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass,MaxMana,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME(ThisClass,bAttributeInitialized);
}

void UGC_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	//检查gameplayeffect影响的data是不是health.并且health<=0
	if (Data.EvaluatedData.Attribute == GetHealthAttribute() && GetHealth()<=0)
	{
		FGameplayEventData Payload;
		Payload.Instigator = Data.Target.GetAvatarActor();
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Data.EffectSpec.GetEffectContext().GetInstigator(),GCTags::GCEvents::KillScored,Payload);
	}
	
	
	if (!bAttributeInitialized) //判断是否是第一次初始化
	{
		bAttributeInitialized = true; //服务器标记初始化成功
		OnAttributeSetInitialized.Broadcast();//通知本地订阅者
	}
}


void UGC_AttributeSet::OnRep_AttributeInitialized()
{
	if (bAttributeInitialized) //判断服务器是否初始化成功
	{
		OnAttributeSetInitialized.Broadcast();//通知客户端订阅者
	}
}
