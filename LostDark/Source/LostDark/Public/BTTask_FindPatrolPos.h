// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPos.generated.h"

/**
 *  AI�� �̵��� ������ PatrolPos ������
 */
UCLASS()
class LOSTDARK_API UBTTask_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	// Constructor
	UBTTask_FindPatrolPos();

	// ExecuteTask : �����̺�� Ʈ������ �½�ũ�� �����Ҷ� ����ϴ� �Լ� �ݵ�� ��� ������ ������� ��ȯ�ؾ��Ѵ�.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // OwnerComp : �����̺�� Ʈ��
};
