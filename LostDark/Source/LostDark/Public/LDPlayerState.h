// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "GameFramework/PlayerState.h"
#include "LDPlayerState.generated.h"

// 
DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);

/**
 * 
 */
UCLASS()
class LOSTDARK_API ALDPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	// Constructor
	ALDPlayerState();
	
	int32 GetGameScore() const;
	int32 GetGameHighScore() const;
	FString SaveSlotName;

	int32 GetCharacterLevel() const;
	
	float GetExpRatio() const;
	bool AddExp(int32 IncomeExp);
	// 
	void AddGameScore();

	void InitPlayerData();
	// ����
	void SavePlayerData();

	// ��������Ʈ ���� ����
	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

protected:
	UPROPERTY(Transient)
		int32 GameScore;
	UPROPERTY(Transient)
		int32 GameHighScore;
	UPROPERTY(Transient)
		int32 CharacterLevel;

	UPROPERTY(Transient)
		int32 Exp;

private:
	// ĳ���� ���� ����
	void SetCharacterLevel(int32 NewCharacterLevel);
	// GameInstance�� �ִ� ����
	struct FGSCharacterData* CurrentStateData;
};
