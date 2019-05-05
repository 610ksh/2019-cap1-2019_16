// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "Blueprint/UserWidget.h"
#include "GSCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOSTDARK_API UGSCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Progress Bar�� ���������� ������Ʈ�� �Լ�.
	void BindCharacterStat(class UGSCharacterStatComponent* NewCharacterStat);

private:
	// �� �����ͷ� ����� �츮 ĳ������ �������� ������Ʈ(Actor Component)
	TWeakObjectPtr<class UGSCharacterStatComponent> CurrentCharacterStat;
};
