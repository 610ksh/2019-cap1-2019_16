// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Detect.h"
//
#include "LDAIController.h" // UBehaviorTreeComponent & OwnerComp ���� OwnerComp�� ������Ʈ ����� ����Ϸ��� �ʿ�
#include "LostDarkCharacter.h" // Ž���� ���� �츮 �÷��̾����� �߷����� �ʿ��� (ALostDarkCharacter* GSCharacter~~)
#include "BehaviorTree/BlackboardComponent.h" // OwnerComp ������Ʈ ����ϰ� ������
#include "DrawDebugHelpers.h" // Ž�� �������� �׸��� ������

// Constructor
UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	// �ֱ������� �Ҹ������� TickNode�� �ð� ����
	Interval = 1.0f;
}

// �ڽ��� ���� ������ ��尡 Ȱ��ȭ�� ��� �ֱ������� TickNode �Լ��� �ڵ� ȣ��
void UBTService_Detect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// ���� �����ϴ� ���� ������ ������
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) return; // �����ϴ� ���� ������ return

	// ���� �����ϴ� ���� ������ ������
	UWorld* World = ControllingPawn->GetWorld();
	// ���� ���� ��ġ���� Center�� ����
	FVector Center = ControllingPawn->GetActorLocation();
	// Ž�� �ݰ� 600cm
	float DetectRadius = 600.0f;
	// ���� ���� ������ ���ٸ� return
	if (World == nullptr) return;
	// �ݰ泻�� �ִ� ��� ĳ���� ������ ���� ������ �ڷᱸ��.
	TArray<FOverlapResult> OverlapResults;
	// CollisionQueryParams�� ���� ���� 
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	// �ݰ泻 ��� ĳ���͸� �����ϴ� �Լ�. bool������ ó����.
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);


	/* Ž��������, �÷��̾ �߷����� ���� */
	if (bResult) // ��� ����� Pawn�̶� �����ߴٸ�,
	{
		// OverlapResults�� ��� ��� �������� �ϳ��� ���������� �˻��ϴµ�,
		for (auto OverlapResult : OverlapResults)
		{
			// ���� Ž���� ������ ���� ����(��, ĳ����)�� ���
			ALostDarkCharacter* GSCharacter = Cast<ALostDarkCharacter>(OverlapResult.GetActor());
			// ���� �� ĳ������ ��Ʈ�ѷ��� PlayerController��� (����ڶ��)
			if (GSCharacter&&GSCharacter->GetController()->IsPlayerController())
			{
				// ������ TargetKey ���� ����� ���� ������ ���� ĳ���ͷ� ���� (����ڷ� Ÿ�� ����)
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ALDAIController::TargetKey, GSCharacter);
				// ������ �� ������ �׸�.
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				// �÷��̾���� ������ ���� �׸�
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), GSCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}
		}
	}
	// ���� �ƹ��͵� �߰����� ��������
	else
	{
		// �ٽ� �������� TargetKey���� nullptr�� �־���. (ã�Ҵٰ�, �÷��̾ ���������� ������ ��츦 ���)
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ALDAIController::TargetKey, nullptr);
	}

	// ������ ��
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
