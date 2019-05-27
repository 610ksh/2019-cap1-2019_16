// Fill out your copyright notice in the Description page of Project Settings.

#include "LDGamePlayWidget.h"
//
#include "Components/Button.h"
#include "LostDarkPlayerController.h"

// �ش� ������ Add to Viewport ������ ȣ���
void ULDGamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// �����
	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (ResumeButton != nullptr)
	{
		ResumeButton->OnClicked.AddDynamic(this, &ULDGamePlayWidget::OnResumeClicked);
	}

	// Ÿ��Ʋ ȭ������
	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (ReturnToTitleButton != nullptr)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &ULDGamePlayWidget::OnReturnToTitleClicked);
	}

	// �絵��
	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (RetryGameButton != nullptr)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &ULDGamePlayWidget::OnRetryGameClicked);
	}
}

// �����
void ULDGamePlayWidget::OnResumeClicked()
{
	// ���� ���忡�� �ڽ��� �����ϴ� �÷��̾� ��Ʈ�ѷ��� ������ �����´�.
	auto LDPlayerController = Cast<ALostDarkPlayerController>(GetOwningPlayer());
	ABCHECK(LDPlayerController != nullptr);

	// ����Ʈ�� ����� �ڽ�(�÷��̾� ��Ʈ�ѷ�)�� ������ �� �ִ�.
	RemoveFromParent();
	LDPlayerController->ChangeInputMode(true);
	LDPlayerController->SetPause(false);
}

// Ÿ��Ʋ�� ���ư���
void ULDGamePlayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}

// �ٽõ���
void ULDGamePlayWidget::OnRetryGameClicked()
{
	// ���� ���忡�� �÷��̾� ��Ʈ�ѷ� ������ ������
	auto LDPlayerController = Cast<ALostDarkPlayerController>(GetOwningPlayer());
	ABCHECK(LDPlayerController != nullptr);
	// �ش��ϴ� �÷��̾� ��Ʈ�ѷ� ������ ������ �ٽ� �����
	LDPlayerController->RestartLevel();
}
