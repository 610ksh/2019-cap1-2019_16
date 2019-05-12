// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Detect.generated.h"

/**
 * ���� �� �÷��̾ ���� �ݰ� ���� ������ ������ �߰��ϴ� ����� �ϴ� ���񽺳��.
 */
UCLASS()
class LOSTDARK_API UBTService_Detect : public UBTService
{
	GENERATED_BODY()
	
public:
	// Constructor
	UBTService_Detect();

protected:
	// �ڽ��� ���� ������ ��尡 Ȱ��ȭ�� ��� �ֱ������� TickNode �Լ��� �ڵ� ȣ��
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
