// Fill out your copyright notice in the Description page of Project Settings.

#include "GSCharacterWidget.h"
// ���� ������Ʈ�� �����ϱ� ���� ��� �߰�
#include "GSCharacterStatComponent.h"
// ProgressBar �����͸� ���� ���
#include "Components/ProgressBar.h"

// ĳ������ PostInitializeComponents �Լ����� ĳ���� ������Ʈ�� UI ������ �����Ҷ� ����� �Լ�.
void UGSCharacterWidget::BindCharacterStat(UGSCharacterStatComponent* NewCharacterStat)
{
	// ĳ���� ���� ������ ����� ������ ������ ����ó��
	ABCHECK(nullptr != NewCharacterStat);

	// �Ѿ�� �����͸� �� �����ͷ� ���� ��� ������ ����.
	CurrentCharacterStat = NewCharacterStat;
	// HP�� ��������, ���ʿ��� �Ҹ�����. (UGSCharacterStatComponent::SetHP(float NewHP) ����Ҷ� ��ε�ĳ��Ʈ �ߵ�)
	//NewCharacterStat->OnHPChanged.AddLambda([this]()->void {
	//	// �� �����͸� ����߱� ������, �ݵ�� IsValid�� ��ȿ���� üũ. (ĳ���� ��������)
	//	if (CurrentCharacterStat.IsValid())
	//	{
	//		// �α� ����.
	//		ABLOG(Warning, TEXT("HPRatio : %f"), CurrentCharacterStat->GetHPRatio());
	//		ABLOG(Warning, TEXT("GOOOOOOOOOOOOOOOOOOOOOOOOOD"));
	//	}
	//});
	
	// ��������Ʈ �Լ� ���
	NewCharacterStat->OnHPChanged.AddUObject(this, &UGSCharacterWidget::UpdateHPWidget);
}

// UI �ý����� �Ϸ�Ǹ� �ڵ�ȣ�� /// ������ UI �ʱ�ȭ (BeginPlay���� �����. ����)
void UGSCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// ������ ���� ���� �̸����� �ּ� ������ �ѱ�
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	// ���� ������ ������ �ʴ´ٸ�(ex���� �̸��� �ٸ����)
	ABCHECK(nullptr != HPProgressBar);
	// ������Ʈ �����Լ� ȣ��
	UpdateHPWidget();
}

// ���� ������ ������ ������Ʈ�ϴ� �Լ�
void UGSCharacterWidget::UpdateHPWidget()
{
	// �������ͷ� ����� ���������� ���� ��ȿ����
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			// ���� HP�� ������ HPProgressBar�� ������Ʈ (SetPercent�Լ���)
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
