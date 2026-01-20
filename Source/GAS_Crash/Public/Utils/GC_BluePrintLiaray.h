#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GC_BluePrintLiaray.generated.h"

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Front,
	Back,
	Left,
	Right
};

UCLASS()
class GAS_CRASH_API UGC_BluePrintLiaray : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	static EHitDirection GetHitDirection(const FVector& TargetForward,const FVector& ToInstigator);
	
	UFUNCTION(BlueprintPure)
	static FName GetHitReactionName(const EHitDirection& HitDirection);
};
