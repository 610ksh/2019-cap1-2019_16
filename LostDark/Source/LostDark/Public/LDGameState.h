// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "GameFramework/GameStateBase.h"
#include "LDGameState.generated.h"

/**
 * 
 */
UCLASS()
class LOSTDARK_API ALDGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	// Constructor
	ALDGameState();

public:
	int32 GetTotalGameScore() const;
	void AddGameScore();
	// ���� Ŭ���� ����
	void SetGameCleared();
	// ���� Ŭ���� ����
	bool IsGameCleared() const;

private:
	// ��ü ���� ���ھ�
	UPROPERTY(Transient)
		int32 TotalGameScore;

	// ���� Ŭ���� ����
	UPROPERTY(Transient)
		bool bGameCleared;
};
