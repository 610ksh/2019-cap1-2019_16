// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "Blueprint/UserWidget.h"
#include "GSCharacterWidget.generated.h"

/**
 *  UserWidget Ŭ������ Anim Instance�� ����������, UI�ʿ��� ������ �����ϴ� Ŭ�����̴�. �𸮾󿡼� �̸� ��������.
 *	UI�� �����͸� �����ϰ� ��ȣ�ۿ��ϴ°� ���� ���⼭ �̷����°���.
 *	���⼭�� ĳ���� ���� ������ ����Ǿ� �ִ� GSCharacterStatComponent (Actor Component)�� ������,
 *	ĳ������ ������ ��ȭ�� ������ Progress Bar�� ������ ������Ʈ�� ����. (�����ʿ� ��������Ʈ�� �����ؼ�, �����.)
 */
UCLASS()
class LOSTDARK_API UGSCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Progress Bar�� ���������� ������Ʈ�� �Լ�.
	void BindCharacterStat(class UGSCharacterStatComponent* NewCharacterStat);

protected:
	// UI�ý����� �غ�Ǹ� �ڵ�ȣ���. ����� UI ������ �÷��̾���Ʈ�ѷ��� BeginPlay()���� �̷���. Postinitialize�� ���� ó���Ǵ°� ����
	virtual void NativeConstruct() override; /// ���������� ���⼭ ������Ʈ�� ����.
	// ���� ������Ʈ
	void UpdateHPWidget();

private:
	// �� �����ͷ� ����� �츮 ĳ������ �������� ������Ʈ(Actor Component) ��, �������� ������ ����Ҷ��� �ݵ�� IsValid�� ��ȿ���� Ȯ��������Ѵ�.
	TWeakObjectPtr<class UGSCharacterStatComponent> CurrentCharacterStat;

	UPROPERTY()
		// Progress Bar UI ������ ����
		class UProgressBar* HPProgressBar;
};
