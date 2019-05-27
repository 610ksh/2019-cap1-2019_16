// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "GameFramework/PlayerController.h"
#include "LostDarkPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LOSTDARK_API ALostDarkPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	// Constructor
	ALostDarkPlayerController();

	//virtual void PostInitializeComponents() override;
	//virtual void Possess(APawn* aPawn) override;

	class ULDHUDWidget* GetHUDWidget() const;

	void AIKill(class ALostDarkCharacter* KiiledAI) const;
	// ���� ���ھ �ø���
	void AddGameScore() const;
	// Input��带 ����
	void ChangeInputMode(bool bGameMode = true);
	
	// ��� UI
	void ShowResultUI();

protected:
	virtual void BeginPlay() override;
	// �÷��̾� ��Ʈ�ѷ��� �ִ� �Լ�. 
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class ULDHUDWidget> HUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class ULDGamePlayWidget> MenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class ULDGamePlayResultWidget> ResultWidgetClass;

private:
	// �������� �Լ�
	void OnGamePause();

	UPROPERTY()
		class ULDHUDWidget* HUDWidget;
	UPROPERTY()
		class ALDPlayerState* LDPlayerState;

	UPROPERTY()
		class ULDGamePlayWidget* MenuWidget;
	UPROPERTY()
		class ULDGamePlayResultWidget* ResultWidget;

	// ���ӿ��� Ŭ���� ����
	FInputModeGameOnly GameInputMode;
	// UI���� Ŭ���� ����
	FInputModeUIOnly UIInputMode;
};
