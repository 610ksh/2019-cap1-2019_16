// Fill out your copyright notice in the Description page of Project Settings.

#include "LDPlayerState.h"
// �߰�
#include "LDGameInstance.h"
#include "LDSaveGame.h"

// Constructor
ALDPlayerState::ALDPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	GameHighScore = 0;
	Exp = 0;
	SaveSlotName = TEXT("Player1");
}

int32 ALDPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 ALDPlayerState::GetGameHighScore() const
{
	return GameHighScore;
}

int32 ALDPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

void ALDPlayerState::InitPlayerData()
{
	// ó�� ������Ʈ �����Ҷ� ���� ���Կ��� ������
	auto LDSaveGame = Cast<ULDSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == LDSaveGame)
	{
		LDSaveGame = GetMutableDefault<ULDSaveGame>();
	}

	// ���̺� ���Ͽ��� �÷��̾� �̸�������
	SetPlayerName(LDSaveGame->PlayerName);
	// ���̺� ���Ͽ��� ĳ���� ���� ������
	SetCharacterLevel(LDSaveGame->Level);
	GameScore = 0;
	GameHighScore = LDSaveGame->HighScore;
	// ���̺� ���Ͽ��� Exp������
	Exp = LDSaveGame->Exp;
	// ���� ����
	SavePlayerData();
}

void ALDPlayerState::SavePlayerData()
{
	// ���̺� �ν��Ͻ� ����
	ULDSaveGame* NewPlayerData = NewObject<ULDSaveGame>();
	// ���� ������Ʈ�� �ش��ϴ� ������ ���Ӱ� ����.
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->HighScore = GameHighScore;

	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		ABLOG(Error, TEXT("SaveGame Error!"));
	}
}

float ALDPlayerState::GetExpRatio() const
{
	// ���� ������ NextExp�� 0���� �۴ٸ�
	if (CurrentStateData->NextExp <= KINDA_SMALL_NUMBER)
		return 0.0f;
	// ����ġ �� ����(%) ���. ���� ���� ����ġ / ��ü ����ġ 
	float Result = (float)Exp / (float)CurrentStateData->NextExp;
	// ����, ����, ��ü
	ABLOG(Warning, TEXT("Ratio : %f, Current : %d, Next : %d"), Result, Exp, CurrentStateData->NextExp);
	return Result;
}

bool ALDPlayerState::AddExp(int32 IncomeExp)
{
	// ���� ����ġ�� ���ٸ�  (�ִ� ����)
	if(CurrentStateData->NextExp==-1)
		return false;

	bool DidLevelup = false;
	Exp = Exp + IncomeExp;
	// ����ġ�� �ִ� ����ġ�� �Ѿ�ٸ�
	if (Exp >= CurrentStateData->NextExp)
	{
		// ��� ����ġ �ʱ�ȭ
		Exp -= CurrentStateData->NextExp;
		// ������ 1
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelup = true;
	}

	// ������Ʈ ����
	OnPlayerStateChanged.Broadcast();
	// ����ġ�� ���������� ������ ����
	SavePlayerData();
	return DidLevelup;
}

void ALDPlayerState::AddGameScore()
{
	GameScore++;
	if (GameScore >= GameHighScore)
	{
		GameHighScore = GameScore;
	}
	OnPlayerStateChanged.Broadcast();
	// ������ ���������� ������ ����
	SavePlayerData();
}

void ALDPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto LDGameInstance = Cast<ULDGameInstance>(GetGameInstance());
	ABCHECK(nullptr != LDGameInstance);

	// ���� ���� �����͸� �����ν��Ͻ����� ������
	CurrentStateData = LDGameInstance->GetGSCharacterData(NewCharacterLevel);
	ABCHECK(nullptr != CurrentStateData);

	CharacterLevel = NewCharacterLevel;
}