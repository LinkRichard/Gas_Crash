#pragma once
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Components/WidgetComponent.h"
#include "GC_WidgetComponent.generated.h"


class UAbilitySystemComponent;
class AMyBaseCharacter;
class UGC_AbilitySystemComponent;
class UGC_AttributeSet;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_CRASH_API UGC_WidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;
	
	//TMap:实现配置驱动,不硬编码属性关系,优势:策划可直接配置,无需程序员修改代码
	// 编辑器配置示例:
	// Key: Health  | Value: MaxHealth
	// Key: Mana    | Value: MaxMana
	UPROPERTY(EditAnywhere)
	TMap<FGameplayAttribute,FGameplayAttribute> AttributeMap;
private:
	
	TWeakObjectPtr<UGC_AttributeSet> AttributeSet;
	TWeakObjectPtr<UGC_AbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<AMyBaseCharacter> BaseCharacter;
	
	// First Initialization
	void InitAbilitySystemData();
	
	// Check if the ASC is Initialized Successfully
	bool IsASCInitialized() const;
	
	//Delegate Callback Function used to Initialize ASC and AS. if first Initialization failed
	UFUNCTION()
	void CBF_ASCInitialized(UAbilitySystemComponent* ASC,UAttributeSet* AS);
	
	//Check if the AttributeSet has been initialized with the fist GE
	void InitAttributeSetDelegate();
	
	//Initialize Attribute to AttributeWidget.Listen actual AttributesSet changes to update WidgetComponent changes.
	UFUNCTION()
	void CBF_BindAllWidgetAttributesChanges();
	
	//Helper function: Binds a specific widget instance to the provided attribute pair.
	void BindSingleWidgetToAttributeChanges(UWidget* WidgetObject,const TTuple<FGameplayAttribute,FGameplayAttribute>& Pair);
};
