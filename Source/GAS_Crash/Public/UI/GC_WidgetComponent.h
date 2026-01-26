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
	
	//TMap是什么?
	UPROPERTY(EditAnywhere)
	TMap<FGameplayAttribute,FGameplayAttribute> AttributeMap;
private:
	
	TWeakObjectPtr<UGC_AttributeSet> AttributeSet;
	TWeakObjectPtr<UGC_AbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<AMyBaseCharacter> BaseCharacter;
	
	// First Initialization
	void InitAbilitySystemData();
	
	//check if the AttributeSet has been initialized with the fist GE and Initialized by Delegate CallBack
	void InitAttributeSetDelegate();
	
	// check if Init Successful
	bool IsASCInitialized() const;
	
	void BindWidgetToAttributeChanges(UWidget* WidgetObject,const TTuple<FGameplayAttribute,FGameplayAttribute>& Pair);
	
	//Delegate Callback Function used to Initialize ASC and AS. if first Initialization failed
	UFUNCTION()
	void CBF_ASCInitialized(UAbilitySystemComponent* ASC,UAttributeSet* AS);
	
	//Listen actual AttributesSet changes to update WidgetComponent changes.
	UFUNCTION()
	void CBF_BindToAttributeChanges();
};
