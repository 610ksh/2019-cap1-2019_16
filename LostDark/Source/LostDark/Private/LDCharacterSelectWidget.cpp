// Fill out your copyright notice in the Description page of Project Settings.

#include "LDCharacterSelectWidget.h"
//
#include "LDCharacterSetting.h"
#include "LDGameInstance.h"
#include "EngineUtils.h" // TActorIterator
#include "Animation/SkeletalMeshActor.h" //ASkeletalMeshActor
#include "Components/Button.h"// Button.OnClicked.AddDynamic()
#include "Components/EditableTextBox.h" // TextBox->GetText().ToString()
#include "LDSaveGame.h"
#include "LDPlayerState.h"

// Add to viewport or root �Ҷ� ȣ��Ǵ� �Լ�. ����Ʈ�� �߰��Ҷ� �����. => ���� UI �����ϰ� ����
void ULDCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentIndex = 0;
	// �ּ� ���� ������ ������.
	auto CharacterSetting = GetDefault<ULDCharacterSetting>();
	// �ּ� ���� ������ (�������� �о��)
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	// ���忡 �ִ� ��� ���̷�Ż �޽ô� TActorIterator�� �����ü� ����. ��, ������ ��ġ�� ���̷�Ż �Ž��� ����ϴ� ������ ���͸� ã�°�.
	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)
	{
		TargetComponent = It->GetSkeletalMeshComponent();
		break;
	}


	/* �� ��ư�� ���ε�, ���̳��� ��������Ʈ ���� */

	// Button
	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	ABCHECK(PrevButton != nullptr);
	// Button
	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	ABCHECK(NextButton != nullptr);
	// Text Box
	TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	ABCHECK(TextBox != nullptr);
	// Button
	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	ABCHECK(ConfirmButton != nullptr);

	PrevButton->OnClicked.AddDynamic(this, &ULDCharacterSelectWidget::OnPrevClicked);
	NextButton->OnClicked.AddDynamic(this, &ULDCharacterSelectWidget::OnNextClicked);
	ConfirmButton->OnClicked.AddDynamic(this, &ULDCharacterSelectWidget::OnConfirmClicked);
}

void ULDCharacterSelectWidget::OnPrevClicked()
{
	NextCharacter(false);
}

void ULDCharacterSelectWidget::OnNextClicked()
{
	NextCharacter(true);
}

// ĳ���͸� ���� ��ư Ŭ��������
void ULDCharacterSelectWidget::NextCharacter(bool bForward)
{
	bForward ? CurrentIndex++ : CurrentIndex--;

	// Queue ����. ������ -> ó��, ó�� ���� -> ������
	if (CurrentIndex == -1) CurrentIndex = MaxIndex - 1;
	if (CurrentIndex == MaxIndex) CurrentIndex = 0;

	// �ּ� ���� ������ ������.
	auto CharacterSetting = GetDefault<ULDCharacterSetting>();
	// ���ʴ� 0��° �޽� ������� ������
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];

	// ���� ������ �����ν��Ͻ� ������ ������ (���� �Ŵ���)
	auto LDGameInstance = GetWorld()->GetGameInstance<ULDGameInstance>();
	ABCHECK(nullptr != LDGameInstance);
	ABCHECK(TargetComponent.IsValid());

	// ���ÿ� ����� �ּ� ��η� �޽ø� ���Ӱ� �ε���. ���̷�Ż �޽� ���� ��������
	USkeletalMesh* Asset = LDGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);
	if (nullptr != Asset)
	{
		// ȭ�鿡 �ִ� ������ �Ž��� ��ü(���̷�Ż�� �����ϱ� ������ ������)
		TargetComponent->SetSkeletalMesh(Asset);
	}

}

void ULDCharacterSelectWidget::OnConfirmClicked()
{
	// �ؽ�Ʈ�ڽ��� �ִ� ���ڿ� ��������
	FString CharacterName = TextBox->GetText().ToString();
	// �̸��� ���ų�, 10���� �̻��̸� �ƹ��ϵ� ����.
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10) return;

	/* ���̺� ��� */
	// ���ο� ���̺� ������ ���� ����.
	ULDSaveGame* NewPlayerData = NewObject<ULDSaveGame>();
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 3;
	NewPlayerData->Exp = 0;
	NewPlayerData->HighScore = 0; // ������� �ִ� Ŭ������ Ƚ��(���� Ƚ��)
	NewPlayerData->CharacterIndex = CurrentIndex; // �ּ� ����� �ε����� ����

	// �÷��̾� ������Ʈ ���� ����
	auto LDPlayerState = GetDefault<ALDPlayerState>();
	// ���� ���̺� �����ϰ�, �����ϸ� // (����������, �����̸�, �����ε���)
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, LDPlayerState->SaveSlotName, 2))
	{
		// ���ο� ������ ����. 2��° ���ڿ��� �� �ּ� �̸��� �־�����Ѵ�.
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Elemental"));
	}
	else
	{
		ABLOG(Error, TEXT("SaveGame Error!"));
	}
}