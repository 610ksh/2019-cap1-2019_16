// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LostDarkGameMode.h"
#include "LostDarkCharacter.h"
//#include "UObject/ConstructorHelpers.h" //���� �ڵ�

// GameMode�� ���λ� A
ALostDarkGameMode::ALostDarkGameMode()
{
	// set default pawn class to our Blueprinted character(3��Ī ����ŷ). Auto Possesss Player 0���� �켱������ �� ����.
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));

	// �ܺ� �������Ʈ Ŭ������ ������ ����Ҷ�, ���_C ������ �Ѱ���.
	static ConstructorHelpers::FClassFinder<APawn> BP_PAWN_C(TEXT
	("/Game/ParagonGreystone/Characters/Heroes/Greystone/GreystonePlayerCharacter.GreystonePlayerCharacter_C"));

	if (BP_PAWN_C.Class != NULL)
	{
		DefaultPawnClass = BP_PAWN_C.Class; // Ŭ���� ������ �Ѱ������.
	}
}
