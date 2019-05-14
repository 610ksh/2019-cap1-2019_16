// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "LostDark.h"
#include "GameFramework/GameModeBase.h"
#include "LostDarkGameMode.generated.h"

/*
	minimalapi = Ŭ���� ������.
	����, ������ ���� ������� ��� �۵��ϴ��� �����.
	Ŭ������ �� ������ �ٸ� ����� ����� �� �ֵ��� ����.
	�� Ŭ������ ����ȯ�� ����������, �� �Լ��� ȣ���� �� ����.
	������ �ð��� ����.
	http://api.unrealengine.com/KOR/Programming/UnrealArchitecture/Reference/Classes/
*/
UCLASS(minimalapi)
class ALostDarkGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	//Constructor
	ALostDarkGameMode();

	//
	virtual void PostInitializeComponents() override;
	// �÷��̾� ��Ʈ�ѷ��� �Ϸ�Ǹ� ���� ��忡�� �ڵ� ȣ��
	virtual void PostLogin(APlayerController* NewPlayer) override;
	// ���� ȹ���� �÷��̾��� ������ �÷��ִ� �Լ�
	void AddScore(class ALostDarkPlayerController* ScoredPlayer);
	// ���� ���� ���ھ�
	int32 GetScore() const;

private:
	// ��ü ���� ������Ʈ (��ü ���� ���ھ ����)
	UPROPERTY()
		class ALDGameState* LDGameState;
};