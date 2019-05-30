// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInAttackRanget.generated.h"

/**
 * ��ǥ���� �÷��̾ ���� ���� ���� �ִ��� �Ǵ��ϴ� ���ڷ�����
 */
UCLASS()
class LOSTDARK_API UBTDecorator_IsInAttackRanget : public UBTDecorator
{
	GENERATED_BODY()
public:
	// Constructor
	UBTDecorator_IsInAttackRanget();

protected:
	// ���ϴ� ������ �޼��ƴ��� �ľ��� �� �ִ�. �� �Լ��� const ����� ���ڷ����� Ŭ������ ��� ���� ���� ������ �� ����.
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
