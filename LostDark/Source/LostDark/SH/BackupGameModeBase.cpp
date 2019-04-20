// Fill out your copyright notice in the Description page of Project Settings.

#include "BackupGameModeBase.h"
#include "BackupCharacter.h"
#include "UObject/ConstructorHelpers.h"

// GameMode�� ���λ� A
ABackupGameModeBase::ABackupGameModeBase()
{
	
	// set default pawn class to our Blueprinted character(3��Ī ����ŷ). Auto Possesss Player 0���� �켱������ �� ����.
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));

	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class; // Ŭ���� ������ �Ѱ������.
	//}

	// �ܺ� �������Ʈ Ŭ������ ������ ����Ҷ�, ���_C ������ �Ѱ���.
	static ConstructorHelpers::FClassFinder<APawn> BP_PAWN_C(TEXT
	("/Game/ParagonGreystone/Characters/Heroes/Greystone/GreystonePlayerCharacter.GreystonePlayerCharacter_C"));

	if (BP_PAWN_C.Class != NULL)
	{
		DefaultPawnClass = BP_PAWN_C.Class; // Ŭ���� ������ �Ѱ������.
	}
}
