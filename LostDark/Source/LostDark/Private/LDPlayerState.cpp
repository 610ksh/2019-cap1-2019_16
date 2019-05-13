// Fill out your copyright notice in the Description page of Project Settings.

#include "LDPlayerState.h"
// �߰�
#include "LDGameInstance.h"

// Constructor
ALDPlayerState::ALDPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	Exp = 0;
}

int32 ALDPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 ALDPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

void ALDPlayerState::InitPlayerData()
{
	// ĳ���� �г��� ����.
	SetPlayerName(TEXT("Destiny"));
	SetCharacterLevel(5);
	GameScore = 0;
	Exp = 0;
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
	return DidLevelup;
}

void ALDPlayerState::AddGameScore()
{
	GameScore++;
	OnPlayerStateChanged.Broadcast();
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