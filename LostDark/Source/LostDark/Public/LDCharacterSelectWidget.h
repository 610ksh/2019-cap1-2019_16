// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "Blueprint/UserWidget.h"
#include "LDCharacterSelectWidget.generated.h"

/**
	Select �ʿ��� �¿� ��ư�� ��������, �޽ø� �ٲ��ִ� ����� �����ϴ� Ŭ����.

 	���� ������ �ִ� ���̷�Ż �޽� ������ ����� ��������
	��ư�� ���������� ���̷�Ż �޽� ������Ʈ�� ������ ���̷�Ż �޽ø� �����ϴ� ����� ����
	���� ���忡 �ִ� Ư�� Ÿ���� ��ӹ��� ������ ����� TActorIterator<����Ÿ��> �������� �����´�
 */
UCLASS()
class LOSTDARK_API ULDCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// �������Ʈ���� �� �Լ��� ����ϱ� ���ؼ�.
	UFUNCTION(BlueprintCallable)
		void NextCharacter(bool bForward = true);

	virtual void NativeConstruct() override;

protected:
	// �޽� �ε���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		int32 CurrentIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		int32 MaxIndex;

	// ������ ��ġ�� ������ ���̷�Ż �Ž� ������Ʈ�� �����ִ� ���͸� ���� ����
	TWeakObjectPtr<USkeletalMeshComponent> TargetComponent;

	UPROPERTY()
		class UButton* PrevButton;
	UPROPERTY()
		class UButton* NextButton;
	UPROPERTY()
		class UEditableTextBox* TextBox;
	UPROPERTY()
		class UButton* ConfirmButton;

private:
	UFUNCTION()
		void OnPrevClicked();
	UFUNCTION()
		void OnNextClicked();
	UFUNCTION()
		void OnConfirmClicked();
};
