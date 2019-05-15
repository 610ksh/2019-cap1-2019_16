// Fill out your copyright notice in the Description page of Project Settings.

#include "LDGamePlayResultWidget.h"
//
#include "Components/TextBlock.h"
#include "LDGameState.h"

void ULDGamePlayResultWidget::BindGameState(ALDGameState* GameState)
{
	ABCHECK(GameState != nullptr);
	// ���ӽ�����Ʈ ������ ���
	CurrentGameState = GameState;
}

// �ش� ������ �߰������� �Ҹ������� �Լ�(Add to Viewport)
void ULDGamePlayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ���ӽ�����Ʈ ������ �̸� ���ε� �ƴ��� üũ
	ABCHECK(CurrentGameState.IsValid());

	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	ABCHECK(Result != nullptr);
	Result->SetText(FText::FromString(CurrentGameState->IsGameCleared() ? TEXT("Mission Complete") : TEXT("Mission Failed")));

	auto TotalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));
	ABCHECK(TotalScore != nullptr);
	TotalScore->SetText(FText::FromString(FString::FromInt(CurrentGameState->GetTotalGameScore())));
}