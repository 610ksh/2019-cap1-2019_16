// Fill out your copyright notice in the Description page of Project Settings.

#include "LDGameInstance.h"

ULDGameInstance::ULDGameInstance()
{
	// ������ ���̺� ���۷����� ������ �־���. ������ string ���� �������
	FString CharacterDataPath = TEXT("/Game/SH/GameData/LostDarkCharacterData.LostDarkCharacterData");
	// ������ ������ ����� ������ ������ ����
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_GSCHARACTER(*CharacterDataPath);
	// ������ ���̺� ������ �� �������� �������� ����ó��
	ABCHECK(DT_GSCHARACTER.Succeeded());
	// ���漱���� ���̺� ���� �ʱ�ȭ. �������� GSCharacterTable�� ����Ѵٴ� �ǹ�.
	GSCharacterTable = DT_GSCHARACTER.Object;
	// �� �ȵǴ��� �𸣰���. RowMap�� �����Ҽ��� ����. �и� �����ϴµ�. Protected�� �׷���?
	//ABCHECK(GSCharacterTable->RowMap.Num()>0);
}

void ULDGameInstance::Init()
{
	Super::Init();
	// Level 20��° ���� DropExp�� ����.
	//ABLOG(Warning,TEXT("DropExp of Level 20 GSCharacter : %d"), GetGSCharacterData(20)->DropExp);
}

// ���� ���� �����Ͱ� ����� ����ü�� ��ȯ���ִ� �Լ�.
FGSCharacterData * ULDGameInstance::GetGSCharacterData(int32 Level)
{
	// ������ ���̺��� ã�� �Լ�
	return GSCharacterTable->FindRow<FGSCharacterData>(*FString::FromInt(Level), TEXT(""));
}
