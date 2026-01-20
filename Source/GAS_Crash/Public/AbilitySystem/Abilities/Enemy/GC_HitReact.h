#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/GC_GameplayAbility.h"
#include "GC_HitReact.generated.h"


UCLASS()
class GAS_CRASH_API UGC_HitReact : public UGC_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly,Category="GC|Abilities")
	FVector TargetForward;
	
	UPROPERTY(BlueprintReadOnly,Category="GC|Abilities")
	FVector ToInstigator;
	
	UFUNCTION(BlueprintCallable,Category="GC|Abilities")
	void CacheHitDirectionVector(AActor* Instigator);
};
