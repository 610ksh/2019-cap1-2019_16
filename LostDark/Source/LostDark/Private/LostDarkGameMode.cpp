// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LostDarkGameMode.h"
#include "LostDarkCharacter.h"
#include "LostDarkPlayerController.h"
#include "LDPlayerState.h"
#include "LDGameState.h"

// GameMode�� ���λ� A
ALostDarkGameMode::ALostDarkGameMode()
{
	// set default pawn class to our Blueprinted character(3��Ī ����ŷ). Auto Possesss Player 0���� �켱������ �� ����.
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));

	/*
		Greystone ������� �������Ʈ Ŭ���� ����Ҷ�.
		�ܺ� �������Ʈ Ŭ������ ������ ����Ҷ�, ���_C ������ �Ѱ���.
	*/
	//static ConstructorHelpers::FClassFinder<APawn> BP_PAWN_C(TEXT
	//("/Game/ParagonGreystone/Characters/Heroes/Greystone/GreystonePlayerCharacter.GreystonePlayerCharacter_C"));

	//if (BP_PAWN_C.Class != NULL)
	//{
	//	DefaultPawnClass = BP_PAWN_C.Class; // Ŭ���� ������ �Ѱ������.
	//}

	// GameMode���� ����Ʈ �� Ŭ���� ���� = LostDarkCharacter C++ Ŭ������ ����
	DefaultPawnClass = ALostDarkCharacter::StaticClass();
	// GameMode���� �÷��̾� ��Ʈ�ѷ� Ŭ���� ����
	PlayerControllerClass = ALostDarkPlayerController::StaticClass();
	// �÷��̾� ������Ʈ ���� ���. �÷��̾� ��Ʈ�ѷ��� �ʱ�ȭ�ɶ� �ش� Ŭ������ �ν��Ͻ��� �����Ǹ鼭 �� �����ι��� PlayerState�Ӽ��� ������.
	PlayerStateClass = ALDPlayerState::StaticClass();
	// ���� ������Ʈ ���� ���.
	GameStateClass = ALDGameState::StaticClass();
}

void ALostDarkGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	LDGameState = Cast<ALDGameState>(GameState);
}

// �÷��̾� ��Ʈ�ѷ��� �Ϸ�Ǹ� ���� ��忡�� �ڵ� ȣ��
void ALostDarkGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// �÷��̾� ��Ʈ�ѷ��� �ش��ϴ� �÷��̾� ������Ʈ ���� ������
	auto LDPlayerState = Cast<ALDPlayerState>(NewPlayer->PlayerState);
	ABCHECK(nullptr != LDPlayerState);
	// ���� ������ ���
	LDPlayerState->InitPlayerData();
}

void ALostDarkGameMode::AddScore(ALostDarkPlayerController * ScoredPlayer)
{
	// �������� �÷��̾� ĳ���� ��Ʈ�ѷ��� ���� �޾ƿ�
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto LDPlayerController = Cast<ALostDarkPlayerController>(It->Get());
		// ���ھ� ȹ���� �÷��̾ ã����
		if ((nullptr != LDPlayerController) && (ScoredPlayer == LDPlayerController))
		{
			LDPlayerController->AddGameScore();
			break;
		}
	}
	// ��ü ���� ������Ʈ�� �÷���
	LDGameState->AddGameScore();
}

int32 ALostDarkGameMode::GetScore() const
{
	return LDGameState->GetTotalGameScore();
}
