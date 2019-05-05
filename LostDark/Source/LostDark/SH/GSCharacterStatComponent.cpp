// Fill out your copyright notice in the Description page of Project Settings.

#include "GSCharacterStatComponent.h"
// ���� �ν��Ͻ� ��� �߰�
#include "LDGameInstance.h"

// Sets default values for this component's properties
UGSCharacterStatComponent::UGSCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// InitializeComponent �����.
	bWantsInitializeComponent = true;

	// ���� ���� 1��
	Level = 1;
	// ...
}


// Called when the game starts
void UGSCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGSCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level); // Level 1�� ��.
}


// Called every frame
void UGSCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGSCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	// ���� ������Ʈ ������ �־��� ���� �ν��Ͻ��� ������ �ʱ�ȭ.
	auto LDGameInstance = Cast<ULDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Ȥ�� ������ �����ν��ͽ��� ���ٸ� (None) ����ó��
	ABCHECK(nullptr != LDGameInstance);
	// Level�� �ش��ϴ� ���̺� ������ ���� ����ü ������ ������.
	CurrentStatData = LDGameInstance->GetGSCharacterData(NewLevel);
	if (nullptr != CurrentStatData)
	{
		// ���� ����
		Level = NewLevel;
		// ���� HP, ���� ������ �ش��ϴ� HP�� ����
		CurrentHP = CurrentStatData->MaxHP;
	}
	// �ش� Level�� ������, ������� 1000 Level �����͸� �����Ϸ��� �Ҷ�.
	else
	{
		//ABLOG(Error, TEXT(("Level (%d) data doesn't exist"), NewLevel);
	}
}

// ������ ��ȣ�ۿ� �� ��ε�ĳ��Ʈ(��������Ʈ)
void UGSCharacterStatComponent::SetDamage(float NewDamage)
{
	// ���� ĳ���� ������ ���̺��� ������ ���� �ʴٸ� ����ó��
	ABCHECK(nullptr != CurrentStatData);
	// ���� HP�� ������ ������ ���, �ּ� 0~�ִ� HP ���̰��� ��������.
	CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP);
	if (CurrentHP <= 0.0f)
	{
		// HP�� 0���Ϸ� �������� ��ε�ĳ��Ʈ
		OnHPIsZero.Broadcast();
	}
}

// ���ݷ� ��ȯ �Լ�
float UGSCharacterStatComponent::GetAttack()
{
	// �����ϰ� �ִ� ���� ������ ���̺� ������ ���ٸ� ����ó��
	ABCHECK(CurrentStatData != nullptr, 0.0f);
	// ���� ������ ���̺� ������ �ִ� Attack���� ��ȯ
	return CurrentStatData->Attack;
}
