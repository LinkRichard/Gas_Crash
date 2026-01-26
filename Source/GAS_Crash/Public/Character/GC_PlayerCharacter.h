#pragma once
#include "CoreMinimal.h"
#include "MyBaseCharacter.h"
#include "GC_PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class GAS_CRASH_API AGC_PlayerCharacter : public AMyBaseCharacter
{
	GENERATED_BODY()

public:
	AGC_PlayerCharacter();
		
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	//PlayerState初始化接口,ASC注册,赋予初始能力等等,只在服务器执行
	virtual void PossessedBy(AController* NewController) override;
	
	//PlayerState Replication 复制接口,只在客户端执行
	virtual void OnRep_PlayerState() override;
	
	virtual UAttributeSet* GetAttributeSet() const override;
	
private:
	UPROPERTY(VisibleAnywhere,Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere,Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;	
};
