// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BackupGameModeBase.generated.h"

/*
	minimalapi = Ŭ���� ������.
	����, ������ ���� ������� ��� �۵��ϴ��� �����.
	Ŭ������ �� ������ �ٸ� ����� ����� �� �ֵ��� ����.
	�� Ŭ������ ����ȯ�� ����������, �� �Լ��� ȣ���� �� ����.
	������ �ð��� ����.
	http://api.unrealengine.com/KOR/Programming/UnrealArchitecture/Reference/Classes/
*/
UCLASS(minimalapi)
class LOSTDARK_API ABackupGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABackupGameModeBase();
	
};
