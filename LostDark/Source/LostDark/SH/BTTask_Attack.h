// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class LOSTDARK_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	// Constructor
	UBTTask_Attack();
	
	// �½�ũ �����Ҷ� ����Լ� �ڵ�ȣ�� 
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// TickTask : �½�ũ������ Tick�Լ�.
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	// �ִϸ��̼� ���� ���� ����
	bool IsAttacking = false;
};
