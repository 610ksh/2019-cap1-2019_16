// Fill out your copyright notice in the Description page of Project Settings.

#include "LostDarkPlayerController.h"
// HUD ����
#include "LDHUDWidget.h"
#include "LDPlayerState.h"
#include "LostDarkCharacter.h"

// Constructor
ALostDarkPlayerController::ALostDarkPlayerController()
{
	//// ���콺 Ŀ�� ���̱� (Whether the mouse cursor should be displayed.)
	//this->bShowMouseCursor = true;
	//// ���콺 Ŭ�� �̺�Ʈ Ȱ��ȭ (Whether actor/component click events should be generated.)
	//this->bEnableClickEvents = true;
	//// ���콺 over �̺�Ʈ = Ŀ���� ������ �߻��ϴ� �̺�Ʈ Ȱ��ȭ (Whether actor/component mouse over events should be generated.)
	//this->bEnableMouseOverEvents = true;

	// �ּ� Class������, _C �ٿ����� �̤�
	static ConstructorHelpers::FClassFinder<ULDHUDWidget> UI_HUD_C(TEXT("/Game/SH/UI/UI_HUD.UI_HUD_C"));
	
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}
}

void ALostDarkPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// ���� ���۽� �ڵ����� ȭ�� �ѹ� Ŭ���Ѱ�ó�� ���۵�
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	// ���� �ּ������� ����, �� ĳ������Ʈ�ѷ��� ������ �����Ѵ�. (�÷��̾� ȭ��)
	HUDWidget = CreateWidget<ULDHUDWidget>(this, HUDWidgetClass);
	// ȭ�鿡 ���
	HUDWidget->AddToViewport();

	// �÷��̾� ������Ʈ�� �޾Ƽ� HUD������ ���ε�
	LDPlayerState = Cast<ALDPlayerState>(PlayerState);
	ABCHECK(nullptr != LDPlayerState);
	HUDWidget->BindPlayerState(LDPlayerState);
	LDPlayerState->OnPlayerStateChanged.Broadcast();
}

// ���� ���� ��ȯ
ULDHUDWidget * ALostDarkPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

// ����ġ ȹ��
void ALostDarkPlayerController::AIKill(ALostDarkCharacter * KiiledAI) const
{
	LDPlayerState->AddExp(KiiledAI->GetExp());
}

void ALostDarkPlayerController::AddGameScore() const
{
	LDPlayerState->AddGameScore();
}
