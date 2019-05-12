// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LostDark/SH/Public/LostDarkGameMode.h"
#include "LostDark/SH/Public/LostDarkCharacter.h"
#include "LostDark/SH/Public/LostDarkPlayerController.h"

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

}
