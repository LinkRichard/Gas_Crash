#pragma once
#include "CoreMinimal.h"
#include "GC_AITypeDefs.generated.h"

/**
 *AI State Enumeration
 */
UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle        UMETA(DisplayName = "待机"),
	Patrolling  UMETA(DisplayName = "巡逻"),
	Chasing     UMETA(DisplayName = "追击"),
	Attacking   UMETA(DisplayName = "攻击"),
	Fleeing     UMETA(DisplayName = "逃跑"),
	Stunned     UMETA(DisplayName = "晕眩")
};

/*
 *Blackboard Keys constants
 * 
 *Best Practice: Use namespaces to avoid magic strings
 *Advantages: Compile-time checks, refactoring safety
 */ 

namespace BBKeys
{
	// ==================== TargetActor ====================
	const FName TargetActor = TEXT("TargetActor");
	
	// ==================== Combats and Status ====================
	const FName bCanAttack = TEXT("bCanAttack");
	const FName bPrimaryAbilityReady = TEXT("bPrimaryAbilityReady");
}
