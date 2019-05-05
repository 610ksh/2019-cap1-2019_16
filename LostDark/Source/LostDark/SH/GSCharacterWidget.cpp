// Fill out your copyright notice in the Description page of Project Settings.

#include "GSCharacterWidget.h"
#include "GSCharacterStatComponent.h"

// ĳ������ PostInitializeComponents �Լ����� ĳ���� ������Ʈ�� UI ������ �����Ҷ� ����� �Լ�.
void UGSCharacterWidget::BindCharacterStat(UGSCharacterStatComponent* NewCharacterStat)
{
	ABLOG(Warning, TEXT("OnHPChanged@@@@@@@@@@@@@@@@@@@@@"));
	// ĳ���� ���� ������ ����� ������ ������ ����ó��
	ABCHECK(nullptr != NewCharacterStat);

	// �Ѿ�� �����͸� �츮�� �̸� ������ ������ ����.
	CurrentCharacterStat = NewCharacterStat;
	// HP�� ��������, ���ʿ��� �Ҹ�����. (UGSCharacterStatComponent::SetHP(float NewHP) ����Ҷ� ��ε�ĳ��Ʈ �ߵ�)
	NewCharacterStat->OnHPChanged.AddLambda([this]()->void {
		// ĳ���� ���� ������ ��ȿ�ϴٸ�,
		if (CurrentCharacterStat.IsValid())
		{
			// �α� ����.
			ABLOG(Warning, TEXT("HPRatio : %f"), CurrentCharacterStat->GetHPRatio());
		}
	});
}