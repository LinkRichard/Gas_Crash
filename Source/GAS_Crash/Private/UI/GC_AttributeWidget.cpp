#include "UI/GC_AttributeWidget.h"

bool UGC_AttributeWidget::MatchesAttributes(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair)
{
	return Attribute == Pair.Key && MaxAttribute == Pair.Value;
}

void UGC_AttributeWidget::OnAttributesChange(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair,
	UGC_AttributeSet* AttributeSet)
{
	const float Value = Pair.Key.GetNumericValue(AttributeSet);
	const float MaxValue = Pair.Value.GetNumericValue(AttributeSet);
	
	BP_OnAttributesChange(Value,MaxValue);
}
