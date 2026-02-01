#include "Utils/GC_BluePrintLibrary.h"

#include "Character/MyBaseCharacter.h"
#include "Kismet/GameplayStatics.h"

EHitDirection UGC_BluePrintLibrary::GetHitDirection(const FVector& TargetForward, const FVector& ToInstigator)
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

FName UGC_BluePrintLibrary::GetHitReactionName(const EHitDirection& HitDirection)
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

// Find Closest Actor with Tag ,可以优化.
FClosestActorWithTagTResult UGC_BluePrintLibrary::FindClosestActorWithTag(const UObject* WorldContextObject,
	const FVector& Origin, const FName& Tag)
{
	//找到所有的带有Tag的actor
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(WorldContextObject,Tag,ActorsWithTag);
	
	//定义最小距离和最小距离下的actor
	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;
	
	for (AActor* Actor:ActorsWithTag)
	{
		//check if the actor is alive.
		if (!IsValid(Actor)) continue;
		AMyBaseCharacter* BaseCharacter = Cast<AMyBaseCharacter>(Actor);
		if (!IsValid(BaseCharacter) || !BaseCharacter->IsAlive()) continue;
		
		//update the ClosestDistance and Actor 
		const float Distance = FVector::Dist(Origin,Actor->GetActorLocation());
		if (Distance<ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestActor = Actor;
		}
	}
	
	FClosestActorWithTagTResult FinalResult;
	FinalResult.ClosestDistance = ClosestDistance;
	FinalResult.ClosestActor = ClosestActor;
	
	return FinalResult;
}


