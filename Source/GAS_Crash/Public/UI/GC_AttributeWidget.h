#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystem/GC_AttributeSet.h"
#include "GC_AttributeWidget.generated.h"



UCLASS() 
class GAS_CRASH_API UGC_AttributeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="GC|Abilities")
	FGameplayAttribute Attribute;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="GC|Abilities")
	FGameplayAttribute MaxAttribute;
	
	bool MatchesAttributes(const TTuple<FGameplayAttribute,FGameplayAttribute>& Pair);
	
	void OnAttributesChange(const TTuple<FGameplayAttribute,FGameplayAttribute>& Pair,UGC_AttributeSet* AttributeSet);

	UFUNCTION(BlueprintImplementableEvent,meta=(DisplayName = "On Attributes Change"))
	void BP_OnAttributesChange(float NewValue,float NewMaxValue);
};
