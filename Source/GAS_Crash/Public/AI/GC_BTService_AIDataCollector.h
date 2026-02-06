#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GC_BTService_AIDataCollector.generated.h"

/*
 * AI Data Collect Service
 * 
 * Architectural Principles:
 * - Should Not: Make decisions, change AI state, execute actions
 * - Should: Query data, update Blackboard, provide decision basis

 * Responsibilities:
 * - Query combat data (distance, health, ability cooldowns)
 * - Update Blackboard key values
 * - Do not participate in state decisions (handled by BehaviorTree's Selector and Decorators)

 * Performance Optimizations:
 * - Use EnemyCharacter's cached interfaces to avoid iteration
 * - Configurable update frequency (Interval)
 * - Only update necessary key values
 */

UCLASS()
class GAS_CRASH_API UGC_BTService_AIDataCollector : public UBTService
{
	GENERATED_BODY()
public:
	
	UGC_BTService_AIDataCollector();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
protected:
	//===========================Black Board Key Set======================
	
	//TargetActor
	UPROPERTY(EditAnywhere,Category="GC|Blackboard")
	FBlackboardKeySelector TargetActorKey;
	
	//Can Attack
	UPROPERTY(EditAnywhere, Category="GC|Blackboard")
	FBlackboardKeySelector bCanAttackKey;
	
	
	//===========================Config Options===========================
	
	//Enable Distance Check
	UPROPERTY(EditAnywhere,Category="GC|Config")
	bool bEnableDistanceCheck = true;
	
private:
	//Update Distance Data, calculates and updates distance to target,check if it can attack
	void UpdateDistanceData(class UBlackboardComponent* BB,class AGC_EnemyCharacter* EnemyCharacter,AActor* Target);
	
};
