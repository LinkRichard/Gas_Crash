#pragma once

#include "CoreMinimal.h"
#include "GameObjects/GC_EffectPickup.h"
#include "GC_HealthPickup.generated.h"

UCLASS(Blueprintable)
class GAS_CRASH_API AGC_HealthPickup : public AGC_EffectPickup
{
	GENERATED_BODY()

public:
	AGC_HealthPickup();
};
