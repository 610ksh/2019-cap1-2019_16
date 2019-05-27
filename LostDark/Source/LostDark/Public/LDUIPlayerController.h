// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "GameFramework/PlayerController.h"
#include "LDUIPlayerController.generated.h"

/*
 *	UI ���� �÷��̾� ��Ʈ�ѷ� Ŭ����.
	������ ���۵Ǹ� �ش� Ŭ�����κ��� UI�� �����ϰ� �̸� Viewport�� ���� �Է��� UI���� �����ϵ��� �����Ѵ�.
 */
UCLASS()
class LOSTDARK_API ALDUIPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	// ���� Ŭ��������. �������Ʈ���� ���� ���� ���༭ �־��� ������. Cpp���� ������ ����.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UUserWidget> UIWidgetClass; // ���� ���� �������Ʈ�� �� (UMG)

	// ���� �ν��Ͻ�
	UPROPERTY()
		class UUserWidget* UIWidgetInstance;
	
};
