// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TurnToTarget.h"
///
#include "LDAIController.h"
#include "LostDarkCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

// Constructor
UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

// Task Node�� ����ɶ� ȣ��Ǵ� �Լ�
EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AI�� �����ϴ� ���� �����͸� ������
	auto AICharacter = Cast<ALostDarkCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (AICharacter == nullptr) return EBTNodeResult::Failed;

	// �����忡 �ִ� Ÿ�� ������ ������
	auto Target = Cast<ALostDarkCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ALDAIController::TargetKey));
	if (Target == nullptr) return EBTNodeResult::Failed;

	// Ÿ�� ��ġ���� ���� AI��ġ�� ��. ���⺤�͸� ���ϴ°�.
	FVector LookVector = Target->GetActorLocation() - AICharacter->GetActorLocation();
	// Z���� 0��. ĳ���Ͱ��� ���̰� �ٸ��� �־
	LookVector.Z = 0.0f;
	// ���� ���͸� ���ؼ� ȸ���� ����.
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	// AI�� ȸ����Ŵ. RInterpTo(���� ȸ����, ��ǥ ȸ����, DeltaTime, ���� �ӵ�)
	AICharacter->SetActorRotation(FMath::RInterpTo(AICharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));
	return EBTNodeResult::Succeeded;
}