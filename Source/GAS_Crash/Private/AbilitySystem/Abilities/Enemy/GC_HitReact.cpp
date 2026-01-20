#include "AbilitySystem/Abilities/Enemy/GC_HitReact.h"

void UGC_HitReact::CacheHitDirectionVector(AActor* Instigator)
{
	//计算受击者正前方
	TargetForward = GetAvatarActorFromActorInfo()->GetActorForwardVector().GetSafeNormal();
	//计算受击者指向攻击者发动者的单位向量,计算公式通常是：(攻击者位置 - 受击者位置).GetSafeNormal()
	const FVector AvatarLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector InstigatorLocation = Instigator->GetActorLocation();
	ToInstigator = (InstigatorLocation-AvatarLocation).GetSafeNormal();
}
