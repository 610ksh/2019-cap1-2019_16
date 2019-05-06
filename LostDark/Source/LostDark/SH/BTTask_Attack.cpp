// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "LDAIController.h"
#include "LostDarkCharacter.h"

// Constructor
UBTTask_Attack::UBTTask_Attack()
{
	// ƽ �½�ũ Ȱ��ȭ
	bNotifyTick = true;
	// ���������� ����.
	IsAttacking = false;
}

// �½�ũ �����Ҷ� ����Լ� �ڵ�ȣ�� 
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	//�θ𿡰� �Ѱ���
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// ���� �������� ĳ���� ������ ������
	auto GSCharacter = Cast<ALostDarkCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (GSCharacter == nullptr) return EBTNodeResult::Failed;

	// �̰Ŷ����� Attack�Լ��� public�� �Ǿ�����.
	GSCharacter->Attack();
	// ���� �ִϸ��̼� ���� true;
	IsAttacking = true;
	// ĳ���� ��Ÿ�� ������ �ڵ� ȣ��Ǵ� �Լ�
	GSCharacter->OnAttackEnd.AddLambda([this]()->void {
		IsAttacking = false;
	});

	// ���� ������
	return EBTNodeResult::InProgress;
}

// TickTask : �½�ũ������ Tick�Լ�.
void UBTTask_Attack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	// �������� �ƴ϶��
	if (!IsAttacking)
	{
		// �½�ũ�� ��������� �˷��ִ� �Լ�
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
