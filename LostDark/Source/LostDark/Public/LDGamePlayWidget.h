// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "Blueprint/UserWidget.h"
#include "LDGamePlayWidget.generated.h"

/**
 *	UI_Menu �ּ��� ����� Ŭ����
	UI_Result �ּ��� ����� Ŭ����
 */
UCLASS()
class LOSTDARK_API ULDGamePlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// �� ������ ����Ʈ�� �߰������� �Ҹ������� �Լ�
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnResumeClicked();
	UFUNCTION()
		void OnReturnToTitleClicked();
	UFUNCTION()
		void OnRetryGameClicked();
protected:
	// �Ѵ� ����
	UPROPERTY()
		class UButton* ResumeButton;
	// �Ѵ� ����
	UPROPERTY()
		class UButton* ReturnToTitleButton;
	// Result UI���� ���
	UPROPERTY()
		class UButton* RetryGameButton;
};
