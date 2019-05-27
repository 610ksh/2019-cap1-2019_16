// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "LDGamePlayWidget.h"
#include "LDGamePlayResultWidget.generated.h"

/**
 *	LDGamePlayWidget Ŭ������ ��ӹ޴� Ŭ����.
	���� ��� UI�� ǥ����.
 */
UCLASS()
class LOSTDARK_API ULDGamePlayResultWidget : public ULDGamePlayWidget
{
	GENERATED_BODY()
		
public:
	void BindGameState(class ALDGameState* GameState);
	
protected:
	virtual void NativeConstruct() override;

private:
	TWeakObjectPtr<class ALDGameState> CurrentGameState;
};
