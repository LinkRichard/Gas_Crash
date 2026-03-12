#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GC_EffectPickup.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
class UParticleSystemComponent;
class USceneComponent;
class USphereComponent;

UCLASS(Blueprintable)
class GAS_CRASH_API AGC_EffectPickup : public AActor
{
	GENERATED_BODY()

public:
	AGC_EffectPickup();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	bool ApplyPickupEffect(AActor* OtherActor) const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GC|Pickup")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GC|Pickup")
	TObjectPtr<UParticleSystemComponent> ParticleSystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GC|Pickup")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GC|Pickup")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GC|Pickup", meta = (ClampMin = "0.1"))
	float EffectLevel = 1.f;
};
