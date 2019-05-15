// Fill out your copyright notice in the Description page of Project Settings.

#include "LDUIPlayerController.h"
// �߰�
#include "Blueprint/UserWidget.h" // ����


void ALDUIPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// UIWidgetClass�� ������ ����ó��
	ABCHECK(nullptr != UIWidgetClass);
	
	// ���� �ν��Ͻ��� ����
	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	ABCHECK(nullptr != UIWidgetInstance);

	UIWidgetInstance->AddToViewport();

	// UI���� �Է��� ���޵Ǵ� ����
	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	// 
	SetInputMode(Mode);
	// ���콺 Ŀ�� ���̱�
	bShowMouseCursor = true;
}