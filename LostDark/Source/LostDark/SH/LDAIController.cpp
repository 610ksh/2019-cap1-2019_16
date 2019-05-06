// Fill out your copyright notice in the Description page of Project Settings.

#include "LDAIController.h"
// 4.20 ���� ���ķ� ������̼� �޽� �ý����� ����Ϸ��� �߰��ؾ���!
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h" // SimplerMoveToLocation�� ����Ϸ��� �ʼ�
#include "BehaviorTree/BehaviorTree.h" // �����̺�� Ʈ��
#include "BehaviorTree/BlackboardData.h" // ������
#include "BehaviorTree/BlackboardComponent.h" // Blackboard ������Ʈ ��� ����Ϸ��� ������� �߰��ؾ���.

// ������ ����� �̸��� �ʱ�ȭ�� const�� ������Ŵ. ������ �̸��� ������ ��ġ�ؾ߸���. �Ǽ� ����
const FName ALDAIController::HomePosKey(TEXT("HomePos"));
const FName ALDAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ALDAIController::TargetKey(TEXT("Target"));

ALDAIController::ALDAIController()
{
	//// Ÿ�̸� 3��
	//RepeatInterval = 3.0f;

	// �ܺ� ������ �ּ� ���� ������
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/SH/AI/BB_LDCharacter.BB_LDCharacter"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	// �ܺ� �����̺�� Ʈ�� �ּ� ���� ������
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/SH/AI/BT_LDCharacter.BT_LDCharacter"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

// AI�� ���ǵ����� �Ҹ��� �Լ�.
void ALDAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
	//// AI��Ʈ�ѷ��� ���ǵǸ� ���忡�� Ÿ�̸� �����ؼ� TimerHandler�� ����ϰ�, OnRepeaTimer �Լ��� ȣ����. 3�� ��������
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &ALDAIController::OnRepeatTimer, RepeatInterval, true);

	// UseBlackborad() : �����带 ����(binding)��Ű�� �Լ�. �������θ� bool������ ��ȯ
	if (UseBlackboard(BBAsset, Blackboard)) 
	{
		/// Blackboard ������Ʈ ��� ����Ϸ��� ������� �߰��ؾ���.
		// �����忡 �ִ� HomePosKey�� ���� ���� ��ġ�� �ʱ�ȭ ����. (HomePosKey = InPawn->GetActorLocation() �̶�� ���� ����)
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation()); // HomePosKey�� �ش��ϴ� �̸��� ������ ������ ���� ��ġ���� �־���.
		// RunBehaviorTree(�����̺�� Ʈ�� ����) : �����̺�� Ʈ�� ����(binding)��Ű�� �Լ�. �������θ� bool������ ��ȯ
		if (!RunBehaviorTree(BTAsset))
		{
			// �α� ���
			ABLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

//void ALDAIController::UnPossess()
//{
//	Super::UnPossess();
//	// Ÿ�̸� �ʱ�ȭ
//	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
//}

//void ALDAIController::OnRepeatTimer()
//{
//	// ���� ���� ������ ������
//	auto CurrentPawn = GetPawn();
//	// AI��Ʈ�ѷ��� ����� ���� ���ٸ� ����ó��
//	ABCHECK(CurrentPawn != nullptr);
//	// ������̼� Ÿ�� ������ ����� ���� ���忡 ��ġ�� ������̼��� �����.
//	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
//	// Ȥ�� ���忡 ��ġ�� ������̼� �ý����� ������ ����ó��
//	if (NavSystem == nullptr) return;
//
//	// ������̼� �޽ÿ� �ִ� ����ü ����
//	FNavLocation NextLocation;
//	
//	// ������̼� �Ž� �ٿ�尡 �ִ� �ݰ濡�� ������ġ���� 500 �ݰ濡�� ���� ��ġ�� �����Ѵٸ�, ������ �������� NextLoctaion�� �����.
//	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
//	{
//		// �����ϰ� ��ϵ� ���� �������� AI�� �̵��ϰ���.
//		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
//		// �ش� ��ġ�� ���
//		ABLOG(Warning, TEXT("Next Location : %s"), *NextLocation.Location.ToString());
//	}
//}