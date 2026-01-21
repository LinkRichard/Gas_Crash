#include "Character/GC_EnemyCharacter.h"
#include "AbilitySystem/GC_AbilitySystemComponent.h"
#include "AbilitySystem/GC_AttributeSet.h"

AGC_EnemyCharacter::AGC_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AbilitySystemComponent = CreateDefaultSubobject<UGC_AbilitySystemComponent>(FName("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	//Create AttributeSet
	AttributesSet = CreateDefaultSubobject<UGC_AttributeSet>(FName("AttributeSet"));
}


void AGC_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (!IsValid(GetAbilitySystemComponent())) return;
	//Initialize ASC,OwnerActor(数据拥有者)->Enemy,AvatarActor(表现持有者)->Enemy
	GetAbilitySystemComponent()->InitAbilityActorInfo(this,this);
	
	if (!HasAuthority()) return;  //if not in sever,don't give gameability
	GiveStartupAbilities();// inherits from mybasecharacter
}

UAbilitySystemComponent* AGC_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

