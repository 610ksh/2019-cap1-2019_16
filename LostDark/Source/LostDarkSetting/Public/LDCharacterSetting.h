// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LDCharacterSetting.generated.h"

/**
 * ������ ����� ĳ���� �ּ��� ����� �����ϴ� Ŭ����
 */
UCLASS(config=LostDark)
class LOSTDARKSETTING_API ULDCharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
	ULDCharacterSetting();

	// �ּ� ����� ����.
	UPROPERTY(config)
		TArray<FSoftObjectPath> CharacterAssets;
};
