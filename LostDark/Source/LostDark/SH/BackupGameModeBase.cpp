// Fill out your copyright notice in the Description page of Project Settings.

#include "BackupGameModeBase.h"
#include "BackupCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABackupGameModeBase::ABackupGameModeBase()
{
	// set default pawn class to our Blueprinted character(3��Ī ����ŷ). Auto Possesss Player 0���� �켱������ �� ����.
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));

	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class; // Ŭ���� ������ �Ѱ������.
	}
}
