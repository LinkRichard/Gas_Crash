#include "Utils/GC_BluePrintLiaray.h"

EHitDirection UGC_BluePrintLiaray::GetHitDirection(const FVector& TargetForward, const FVector& ToInstigator)
{
	const float Dot = FVector::DotProduct(TargetForward,ToInstigator);
	
	if (Dot>0.5)
	{
		return EHitDirection::Front;
	}
	if (Dot>-0.5)
	{
		const FVector Cross= FVector::CrossProduct(TargetForward,ToInstigator);
		if (Cross.Z>0)
		{
			return EHitDirection::Right;
		}
		return EHitDirection::Left;
	}
	
	return EHitDirection::Back;	
}

FName UGC_BluePrintLiaray::GetHitReactionName(const EHitDirection& HitDirection)
{
	switch (HitDirection)
	{
	case EHitDirection::Back: return FName("Back");
	case EHitDirection::Front: return FName("Front");
	case EHitDirection::Left: return FName("Left");
	case EHitDirection::Right: return FName("Right");
	default: return FName("None");
	}
}
