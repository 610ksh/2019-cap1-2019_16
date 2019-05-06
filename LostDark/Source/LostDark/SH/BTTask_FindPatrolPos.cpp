// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindPatrolPos.h"
/// �߰���
#include "LDAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
// 4.20 �������� �߰��ؾ���
#include "NavigationSystem.h"

// Constructor
UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	// NodeName : �½�ũ �̸�, ���Ӱ� ����. �̶� TEXT() ���ڿ��� �����.
	NodeName = TEXT("FindPatrolPos");
}

//
EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// �θ𿡰� ������ �Ѱܼ� �ޱ�
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// �����̺�� Ʈ������ AI �������� ���� ������ ������
	auto ControlligPawn = OwnerComp.GetAIOwner()->GetPawn();

	// �������� ���� ���ٸ�
	if (ControlligPawn == nullptr)
		return EBTNodeResult::Failed; //���� �������� ���� ���ٸ� ���и� ����

	// ������̼� ������ ���� ��ϵ� ������̼� �޽� ������ ������
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControlligPawn->GetWorld());
	
	// ������̼� ������ ���ٸ�
	if (NavSystem == nullptr)
		return EBTNodeResult::Failed; // ���� ��ϵ� ������̼� ������ ���ٸ� Failed ����

	// AI�� ������ ���� ��ġ�� �����忡�� �����ͼ� ������ �־���
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ALDAIController::HomePosKey);
	// ���� �������� ����� ���� ����
	FNavLocation NextPatrol;

	// ���� ���� ������̼� ������ �����Ѵٸ�, NextPatrol�� ���� �־��ش�.
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		// �����̺�� Ʈ���� �����忡�� ������ PatroPoskey ���� NextPatrol.Location ����Ѵ�. 
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ALDAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
