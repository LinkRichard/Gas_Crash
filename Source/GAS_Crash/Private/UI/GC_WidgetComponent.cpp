#include "UI/GC_WidgetComponent.h"
#include "AbilitySystem/GC_AbilitySystemComponent.h"
#include "AbilitySystem/GC_AttributeSet.h"
#include "Character/MyBaseCharacter.h"
#include "UI/GC_AttributeWidget.h"
#include "Blueprint/WidgetTree.h"

void UGC_WidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//Firstly initialize ASC and AS
	InitAbilitySystemData();
	
	//如果没有初始化成功,就订阅委托,等待PlayerState复制完成,进行委托广播.然后调用回调函数来重新初始化.
	if (!IsASCInitialized())
	{
		BaseCharacter->OnAscInitialized.AddDynamic(this,&ThisClass::CBF_ASCInitialized);
		return;  //不要忘记return不然下方代码多执行一次.
	}
	
	//如果第一次就初始化成功就直接开始检查,初始化属性集
	InitAttributeSetDelegate();
}

void UGC_WidgetComponent::InitAbilitySystemData()
{
	BaseCharacter = Cast<AMyBaseCharacter>(GetOwner());
	AttributeSet = Cast<UGC_AttributeSet>(BaseCharacter->GetAttributeSet());
	AbilitySystemComponent = Cast<UGC_AbilitySystemComponent>(BaseCharacter->GetAbilitySystemComponent()); 
}

bool UGC_WidgetComponent::IsASCInitialized() const
{
	return AbilitySystemComponent.IsValid() && AttributeSet.IsValid();
}

void UGC_WidgetComponent::CBF_ASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// 参数来源:这些参数由BaseCharacter在ASC初始化完成后通过委托传递 : BaseCharacter->OnAscInitialized.Broadcast(MyASC, MyAS);
	AbilitySystemComponent = Cast<UGC_AbilitySystemComponent>(ASC);
	AttributeSet = Cast<UGC_AttributeSet>(AS);
	
	//执行到这里,如果上面赋值成功.那么就开始检查并初始属性集  
	if (!IsASCInitialized()) return;
	InitAttributeSetDelegate();
}


void UGC_WidgetComponent::InitAttributeSetDelegate()
{
	// Check if the AttributeSet has been fully initialized (by the initial GE).
	// If false, it means data is pending. We subscribe to the delegate and wait for the signal.
	if (!AttributeSet->bAttributeInitialized)
	{
		AttributeSet->OnAttributeSetInitialized.AddDynamic(this,&ThisClass::CBF_BindAllWidgetAttributesChanges);
	}else  
	{
		// If data is already ready, proceed to bind immediately.
		CBF_BindAllWidgetAttributesChanges();
	}   
}

void UGC_WidgetComponent::CBF_BindAllWidgetAttributesChanges()
{
	for (const TTuple<FGameplayAttribute,FGameplayAttribute>& Pair : AttributeMap)
	{
		//check the Root Widget first
		//check the Root is container or Single functional Widget
		BindSingleWidgetToAttributeChanges(GetUserWidgetObject(),Pair);
		
		// Iterate through all childWidget in the tree 
		//WidgetTree->ForEachWidget does not include Root Widget.
		GetUserWidgetObject()->WidgetTree->ForEachWidget([this,Pair](UWidget* ChildWidget)
		{
			BindSingleWidgetToAttributeChanges(ChildWidget,Pair);		
		});
	}
}

void UGC_WidgetComponent::BindSingleWidgetToAttributeChanges(UWidget* WidgetObject,
	const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair)
{
	UGC_AttributeWidget* AttributeWidget = Cast<UGC_AttributeWidget>(WidgetObject);
	if (!IsValid(AttributeWidget)) return;
	if (!AttributeWidget->MatchesAttributes(Pair)) return; 
	
	//Step1: Push the initial value ensure the UI is correct before any changes occured
	AttributeWidget->OnAttributesChange(Pair,AttributeSet.Get());
	
	//Step2: Subscribe ASC->Delegate to Update AttributeValue in AttributeWidget when AttributeData changed.
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda(
		[this,AttributeWidget,Pair]
		(const FOnAttributeChangeData& AttributeChangeData)
		{AttributeWidget->OnAttributesChange(Pair,AttributeSet.Get());}
		);
}

