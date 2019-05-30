// Fill out your copyright notice in the Description page of Project Settings.

#include "LostDarkPlayerController.h"
// HUD ����
#include "LDHUDWidget.h"
#include "LDPlayerState.h"
#include "LostDarkCharacter.h"
#include "LDGamePlayWidget.h"
#include "LDGamePlayResultWidget.h"
#include "LDGameState.h"

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

	static ConstructorHelpers::FClassFinder<ULDGamePlayWidget> UI_MENU_C(TEXT("/Game/SH/UI/UI_Menu.UI_Menu_C"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}

	static ConstructorHelpers::FClassFinder<ULDGamePlayResultWidget> UI_RESULT_C(TEXT("/Game/SH/UI/UI_Result.UI_Result_C"));
	if (UI_RESULT_C.Succeeded())
	{
		ResultWidgetClass = UI_RESULT_C.Class;
	}
}

void ALostDarkPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ChangeInputMode(true);
	//// ���� ���۽� �ڵ����� ȭ�� �ѹ� Ŭ���Ѱ�ó�� ���۵�
	//FInputModeGameOnly InputMode;
	//SetInputMode(InputMode);

	if (HUDWidget == nullptr)
	{
		// ���� �ּ������� ����, �� ĳ������Ʈ�ѷ��� ������ �����Ѵ�. (�÷��̾� ȭ��)
		HUDWidget = CreateWidget<ULDHUDWidget>(this, HUDWidgetClass);
		ABCHECK(HUDWidget != nullptr);
		// ȭ�鿡 ���
		HUDWidget->AddToViewport(1);
	}

	// ��� ���� ��������
	ResultWidget = CreateWidget<ULDGamePlayResultWidget>(this, ResultWidgetClass);
	ABCHECK(ResultWidget != nullptr);

	// �÷��̾� ������Ʈ�� �޾Ƽ� HUD������ ���ε�
	LDPlayerState = Cast<ALDPlayerState>(PlayerState);
	ABCHECK(nullptr != LDPlayerState);
	HUDWidget->BindPlayerState(LDPlayerState);
	LDPlayerState->OnPlayerStateChanged.Broadcast();
}

// ���ε�
void ALostDarkPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &ALostDarkPlayerController::OnGamePause);
}

void ALostDarkPlayerController::OnGamePause()
{
	// �޴� ���� ����
	MenuWidget = CreateWidget<ULDGamePlayWidget>(this, MenuWidgetClass);
	ABCHECK(MenuWidget != nullptr);
	MenuWidget->AddToViewport(3);

	// �Ͻ�����
	SetPause(true);
	// UI�� �Է�
	ChangeInputMode(false);
}

// ���� ���� ��ȯ
ULDHUDWidget * ALostDarkPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

// �ܺ� �������Ʈ Enemy���� �׾����� ����ġ�� �޾ƿ��� �Լ�. UFUNCTION
void ALostDarkPlayerController::AddScore(int32 Score) const
{
	ABLOG(Warning, TEXT("New AI Level : %d"), Score);
	LDPlayerState->AddExp(Score);
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

void ALostDarkPlayerController::ChangeInputMode(bool bGameMode)
{
	// true�� ����ȭ�鸸
	if (bGameMode)
	{
		//SetInputMode(GameInputMode);
		bShowMouseCursor = true;
	}
	// false�� UI����
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void ALostDarkPlayerController::ShowResultUI()
{
	// ���� ���忡�� ���ӽ�����Ʈ�� ������
	auto LDGameState = Cast<ALDGameState>(UGameplayStatics::GetGameState(this));
	ABCHECK(LDGameState != nullptr);
	// �ش� ���ӽ�����Ʈ ������ ���ε���Ŵ
	ResultWidget->BindGameState(LDGameState);
	
	// �� ��Ʈ�� �߰�
	ResultWidget->AddToViewport();
	ChangeInputMode(false);
}