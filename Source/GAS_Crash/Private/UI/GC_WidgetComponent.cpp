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

void UGC_WidgetComponent::InitAttributeSetDelegate()
{
	// if initialize attributeset by GE is failed,Use Delegate
	if (!AttributeSet->bAttributeInitialized)
	{
		AttributeSet->OnAttributeSetInitialized.AddDynamic(this,&ThisClass::CBF_BindToAttributeChanges);
	}else  // it means init is finished just need to change attribute when in else
	{
		CBF_BindToAttributeChanges();
	}
}

bool UGC_WidgetComponent::IsASCInitialized() const
{
	return AbilitySystemComponent.IsValid() && AttributeSet.IsValid();
}

void UGC_WidgetComponent::CBF_ASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	AbilitySystemComponent = Cast<UGC_AbilitySystemComponent>(ASC);//这里的参数为啥不是basecharacter->getabilitysystemcomponent()
	AttributeSet = Cast<UGC_AttributeSet>(AS);
	
	//执行到这里,如果上面赋值成功.那么就开始检查并初始属性集  
	if (!IsASCInitialized()) return;
	InitAttributeSetDelegate();
}


void UGC_WidgetComponent::CBF_BindToAttributeChanges()
{
	for (const TTuple<FGameplayAttribute,FGameplayAttribute>& Pair : AttributeMap)
	{
		BindWidgetToAttributeChanges(GetUserWidgetObject(),Pair);
		
		GetUserWidgetObject()->WidgetTree->ForEachWidget([this,&Pair](UWidget* ChildWidget)
		{
			BindWidgetToAttributeChanges(ChildWidget,Pair);		
		});
	}
}

void UGC_WidgetComponent::BindWidgetToAttributeChanges(UWidget* WidgetObject,
	const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair)
{
	UGC_AttributeWidget* AttributeWidget = Cast<UGC_AttributeWidget>(WidgetObject);
	if (!IsValid(AttributeWidget)) return; // only care GC Attribute Widgets
	if (!AttributeWidget->MatchesAttributes(Pair)) return; 
	
	AttributeWidget->OnAttributesChange(Pair,AttributeSet.Get()); //Initialize
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda([this,AttributeWidget,&Pair](const FOnAttributeChangeData& AttributeChangeData){
		AttributeWidget->OnAttributesChange(Pair,AttributeSet.Get());
	});
}

//犯错:直接GetOwner(),BaseCharacter->GetAttribute()忘记Cast<>()了

