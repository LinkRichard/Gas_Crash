#include "Tasks/GC_AttributeChangeTask.h"
#include "AbilitySystemComponent.h"

UGC_AttributeChangeTask* UGC_AttributeChangeTask::ListenAttributeChanged(FGameplayAttribute Attribute,
	UAbilitySystemComponent* AbilitySystemComponent)
{
	UGC_AttributeChangeTask* WaitForAttributeChangeTask = NewObject<UGC_AttributeChangeTask>();
	WaitForAttributeChangeTask->ASC = AbilitySystemComponent;
	WaitForAttributeChangeTask->AttributeToListen = Attribute;
	
	if (!IsValid(AbilitySystemComponent))
	{
		WaitForAttributeChangeTask->RemoveFromRoot();
		return nullptr;
	}
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(WaitForAttributeChangeTask,&ThisClass::CBF_AttributeChanged);
	return WaitForAttributeChangeTask;
}

void UGC_AttributeChangeTask::EndTask()
{
	//
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(AttributeToListen).RemoveAll(this);
	}
	
	//
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UGC_AttributeChangeTask::CBF_AttributeChanged(const FOnAttributeChangeData& Data)
{
	OnAttributeChanged.Broadcast(Data.Attribute,Data.NewValue,Data.OldValue);
}
