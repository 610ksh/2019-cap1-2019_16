// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

/**
 *  ������ �÷��̾ ������ Task Node
 ���� Task Node�� ���� �ִϸ��̼��� ���� ������ ����ؾ� �ϴ� ���� �½�ũ�̹Ƿ�,
 ExcuteTask�� ��� ���� InProgress�� ��ȯ�ϰ� ������ �������� �½�ũ�� �����ٰ� �˷���� �Ѵ�.
 */
UCLASS()
class LOSTDARK_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	// Constructor
	UBTTask_Attack();
	
	// Task ��尡 �����Ҷ� �Ź� �ڵ�ȣ�� 
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// TickTask : �½�ũ������ Tick�Լ�.
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	// �ִϸ��̼� ���� ���� ����
	bool IsAttacking = false;
};
