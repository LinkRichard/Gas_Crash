#include "AbilitySystem/Abilities/Enemy/GC_EnemyAttack.h"
#include "AI/GC_AITypeDefs.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/GC_EnemyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GameObjects/GC_Projectile.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"

UGC_EnemyAttack::UGC_EnemyAttack()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGC_EnemyAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!IsValid(AvatarActor) || !AvatarActor->HasAuthority() || !ProjectileClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const AGC_EnemyCharacter* EnemyCharacter = Cast<AGC_EnemyCharacter>(AvatarActor);
	if (!IsValid(EnemyCharacter) || !EnemyCharacter->IsAlive())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* TargetActor = ResolveTargetActor();

	FTransform SpawnTransform;

	if (!IsValid(TargetActor) || !ResolveSpawnTransform(TargetActor, SpawnTransform))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = AvatarActor;
	SpawnParams.Instigator = Cast<APawn>(AvatarActor);
	SpawnParams.SpawnCollisionHandlingOverride = CollisionHandlingMethod;

	AGC_Projectile* Projectile = GetWorld()->SpawnActorDeferred<AGC_Projectile>(
		ProjectileClass,
		SpawnTransform,
		AvatarActor,
		SpawnParams.Instigator,
		CollisionHandlingMethod);

	if (!IsValid(Projectile))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Projectile->Damage = ProjectileDamage;
	Projectile->FinishSpawning(SpawnTransform);

	if (bDrawDebugs)
	{
		DrawDebugLine(GetWorld(), SpawnTransform.GetLocation(), ResolveTargetAimLocation(TargetActor), FColor::Red, false, 2.0f, 0, 1.5f);
		DrawDebugSphere(GetWorld(), ResolveTargetAimLocation(TargetActor), 10.0f, 12, FColor::Yellow, false, 2.0f);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

AActor* UGC_EnemyAttack::ResolveTargetActor() const
{
	const AGC_EnemyCharacter* EnemyCharacter = GetGC_EnemyCharacter();
	if (!IsValid(EnemyCharacter)) return nullptr;

	const AAIController* AIController = Cast<AAIController>(EnemyCharacter->GetController());
	if (!IsValid(AIController)) return nullptr;

	const UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!IsValid(Blackboard)) return nullptr;

	return Cast<AActor>(Blackboard->GetValueAsObject(BBKeys::TargetActor));
}

bool UGC_EnemyAttack::ResolveSpawnTransform(const AActor* TargetActor, FTransform& OutSpawnTransform) const
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!IsValid(AvatarActor) || !IsValid(TargetActor)) return false;

	const ACharacter* Character = Cast<ACharacter>(AvatarActor);
	if (!IsValid(Character)) return false;

	const USkeletalMeshComponent* MeshComponent = Character->GetMesh();
	if (!IsValid(MeshComponent)) return false;

	const FTransform MuzzleTransform = MeshComponent->GetSocketTransform(MuzzleSocketName, RTS_World);
	const FVector SpawnLocation = MuzzleTransform.GetLocation() + SpawnLocationOffset;
	const FVector AimLocation = ResolveTargetAimLocation(TargetActor);
	const FRotator SpawnRotation = (AimLocation - SpawnLocation).Rotation();

	OutSpawnTransform = FTransform(SpawnRotation, SpawnLocation, MuzzleTransform.GetScale3D());
	return true;
}

FVector UGC_EnemyAttack::ResolveTargetAimLocation(const AActor* TargetActor) const
{
	if (!IsValid(TargetActor)) return FVector::ZeroVector;

	FVector BoundsOrigin = FVector::ZeroVector;
	FVector BoundsExtent = FVector::ZeroVector;
	TargetActor->GetActorBounds(true, BoundsOrigin, BoundsExtent);


	return BoundsOrigin + TargetAimOffset;
}
