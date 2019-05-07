// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
// ���� ������ ����Ϸ���
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "LDGameInstance.generated.h"

// ����ü �������ش�.
USTRUCT(BlueprintType)
// FTableRowBase ����ü�� ��ӹ޾ƾ��Ѵ�.
struct FGSCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// ���̺��� ù��° ��ġ�� �� �����ʹ�(Name) �𸮾󿡼� �ڵ����� Ű ������ ����ϱ� ������ ���𿡼� �����ؾ���. (��Ģ)
	FGSCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}

	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Level;
	// �ִ� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxHP;
	// ���ݷ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Attack;
	// �������� ����ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 DropExp;
	// ���� ����ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 NextExp;
};


/**
 *
 */
UCLASS()
class LOSTDARK_API ULDGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	ULDGameInstance();

	virtual void Init() override;
	// ĳ���� ������ ���̺��� ������ �������� �Լ�. �ش� ���̺� ���� ������ ������. Level�� ���� ��� ���� ���� ����
	FGSCharacterData* GetGSCharacterData(int32 Level);

private:
	// ������ ���̺� ����ϱ� ���� ����. ���� ������ �� ������ ��.
	UPROPERTY()
		class UDataTable* GSCharacterTable;

};
