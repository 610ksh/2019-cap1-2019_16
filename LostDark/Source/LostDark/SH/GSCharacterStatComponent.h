// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "Components/ActorComponent.h"
#include "GSCharacterStatComponent.generated.h"

// �ָ�ĳ��Ʈ ��������Ʈ ����
DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOSTDARK_API UGSCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGSCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// �̸� ������� �̴ϼȶ����� ������Ʈ �������̵�
	virtual void InitializeComponent() override;

public:	
	// Called every frame (������)
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// ���ο� ���� ����
	void SetNewLevel(int32 NewLevel);
	// ĳ������ TakeDamage �Լ����� �̸� ȣ���ؼ� ������ ��ȣ�ۿ��� ó��
	void SetDamage(float NewDamage);
	// ������ ũ�� ��ȯ
	float GetAttack();

	// ��������Ʈ ����
	FOnHPIsZeroDelegate OnHPIsZero;

private:
	// ĳ���� ������ ���̺� ����ü ���� ����
	struct FGSCharacterData* CurrentStatData = nullptr;

	// ���� ����
	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Level;
	// ���� HP / Transient�� UPROPERTY �Ӽ��� �������� �ʴ´ٴ°� �ǹ��Ѵ�. ���÷� ���Ҽ� �ִ� �������̱⿡ �����ϴ°��� �ǹ� ����.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
		float CurrentHP;

		
};
