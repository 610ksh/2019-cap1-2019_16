// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "AIController.h"
#include "LDAIController.generated.h"

/**
 * 
 */
UCLASS()
class LOSTDARK_API ALDAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	// Constructor(���� ctor)
	ALDAIController();
	// AI ��Ʈ�ѷ��� �ִ� ���� �Լ�
	virtual void Possess(APawn* InPawn) override;
	//// AI ��Ʈ�ѷ��� �ִ� Ż�� �Լ�
	//virtual void UnPossess() override;

	// ������ġ�� ������ ��ġ���� ���� ������ Vector���� ������ static const�̹Ƿ� ���� �ٲ�� �ȵ�
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

	void RunAI();
	void StopAI();
	
private:
	//// Ÿ�̸� �Լ�
	//void OnRepeatTimer();
	//// 
	//FTimerHandle RepeatTimerHandle;
	//// Ÿ�̸� �ð� ����
	//float RepeatInterval;

	// �����̺�� Ʈ�� �ּ� ����
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	// ������ �ּ� ����
	UPROPERTY()
		class UBlackboardData* BBAsset;
};
