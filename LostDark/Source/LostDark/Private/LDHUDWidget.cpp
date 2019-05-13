// Fill out your copyright notice in the Description page of Project Settings.

#include "LDHUDWidget.h"
// �߰��� ���
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GSCharacterStatComponent.h"
#include "LDPlayerState.h"

// ĳ���� ���ݰ� HUD ����
void ULDHUDWidget::BindCharacterStat(class UGSCharacterStatComponent* CharacterStat)
{
	ABCHECK(nullptr != CharacterStat);
	CurrentCharacterStat = CharacterStat;
	// ĳ���� ������ OnHPChanged�� �߻��ϸ� �Ʒ� �Լ��� ����
	CharacterStat->OnHPChanged.AddUObject(this, &ULDHUDWidget::UpdateCharacterStat);
}

// ĳ���� ������Ʈ�� HUD ����
void ULDHUDWidget::BindPlayerState(ALDPlayerState * PlayerState)
{
	ABCHECK(nullptr != PlayerState);
	CurrentPlayerState = PlayerState;
	// ĳ���� ������Ʈ�� �ٲ�� �Ʒ� �Լ��� ����
	PlayerState->OnPlayerStateChanged.AddUObject(this, &ULDHUDWidget::UpdatePlayerState);
}

void ULDHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// ������ ���� ������� ������ ������Ʈ���� �������°�.
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	ABCHECK(nullptr != HPBar);

	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExp")));
	ABCHECK(nullptr != ExpBar);

	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));
	ABCHECK(nullptr != PlayerName);

	PlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));
	ABCHECK(nullptr != PlayerLevel);

	CurrentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));
	ABCHECK(nullptr != CurrentScore);

	HighScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtHighScore")));
	ABCHECK(nullptr != HighScore);
}

void ULDHUDWidget::UpdateCharacterStat()
{
	ABCHECK(CurrentCharacterStat.IsValid());
	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
}

void ULDHUDWidget::UpdatePlayerState()
{
	ABCHECK(CurrentPlayerState.IsValid());

	// Exp�� ����
	ExpBar->SetPercent(CurrentPlayerState->GetExpRatio());
	PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
	PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
	CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameScore())));
}
