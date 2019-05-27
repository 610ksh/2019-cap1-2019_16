// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsInAttackRanget.h"
//
#include "LDAIController.h"
#include "LostDarkCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


// Constructor
UBTDecorator_IsInAttackRanget::UBTDecorator_IsInAttackRanget()
{
	NodeName = TEXT("CanAttack");
}

// �ش��ϴ� Composites�� ����ɶ� �ڵ����� �Ҹ������� �Լ�. ���� Ȯ��
bool UBTDecorator_IsInAttackRanget::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// AI�� �������� ���� ������ ������
	auto ControllingPawn = Cast<ALostDarkCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) return false;
	
	// ���� �����忡 �� �ִ� Target ���� ������
	auto Target = Cast<ALostDarkCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ALDAIController::TargetKey));
	if (Target == nullptr) return false;
	
	// AI���� Ÿ����� �Ÿ��� 200cm �̳���� true�� �Ѱ��ش�.
	bResult = (Target->GetDistanceTo(ControllingPawn) <= ControllingPawn->GetFinalAttackRange());

	return bResult;
}