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

void ULDCharacterSelectWidget::NextCharacter(bool bForward)
{
	bForward ? CurrentIndex++ : CurrentIndex--;

	// Queue ����. ������ -> ó��, ó�� ���� -> ������
	if (CurrentIndex == -1) CurrentIndex = MaxIndex - 1;
	if (CurrentIndex == MaxIndex) CurrentIndex = 0;

	auto CharacterSetting = GetDefault<ULDCharacterSetting>();
	// �ʱ�ȭ ���� ������ 0�̹Ƿ� ���� �ּ� ���� ������.
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];

	// ���� ������ �����ν��Ͻ� ������ ������ (���� �Ŵ���)
	auto LDGameInstance = GetWorld()->GetGameInstance<ULDGameInstance>();
	ABCHECK(nullptr != LDGameInstance);
	ABCHECK(TargetComponent.IsValid());

	// �ּ� ����� ù��° �ּ��� �ε���. ���̷�Ż �޽� ���� ��������
	USkeletalMesh* Asset = LDGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);
	if (nullptr != Asset)
	{
		TargetComponent->SetSkeletalMesh(Asset);
	}

}

// Add to viewport or root �Ҷ� ȣ��Ǵ� �Լ�. ����Ʈ�� �߰��Ҷ� �����.
void ULDCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentIndex = 0;
	auto CharacterSetting = GetDefault<ULDCharacterSetting>();
	// �ּ� ���� ������
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	// ���忡 �ִ� ��� ���̷�Ż �޽ø� ���� ���͸� ������. �װ� Null�� �ɶ�����
	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)
	{
		TargetComponent = It->GetSkeletalMeshComponent();
		break;
	}

	/* ��ư�� ���ε� ��Ų��, ���̳��� ��������Ʈ ���� */
	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	ABCHECK(PrevButton != nullptr);

	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	ABCHECK(NextButton != nullptr);

	TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	ABCHECK(TextBox != nullptr);

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
void ULDCharacterSelectWidget::OnConfirmClicked()
{
	// �ؽ�Ʈ�ڽ��� �ִ� ���ڿ� ��������
	FString CharacterName = TextBox->GetText().ToString();
	// �̸��� ���ų�, 10���� �̻��̸� �ƹ��ϵ� ����.
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10) return;

	// ���ο� ���̺� ������ ���� ����.
	ULDSaveGame* NewPlayerData = NewObject<ULDSaveGame>();
	NewPlayerData->PlayerName = CharacterName;
	// ���� ���� ���
	NewPlayerData->Level = 5;
	NewPlayerData->Exp = 50;
	NewPlayerData->HighScore = 0;
	NewPlayerData->CharacterIndex = CurrentIndex;

	// �÷��̾� ������Ʈ ���� ����
	auto LDPlayerState = GetDefault<ALDPlayerState>();
	// ���� ���̺� �����ϰ�, �����ϸ�
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, LDPlayerState->SaveSlotName, 0))
	{
		// ���ο� ������ ����. 2��° ���ڿ��� �� �ּ� �̸��� �־�����Ѵ�.
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("SH_DefaultLevel"));
	}
	else
	{
		ABLOG(Error, TEXT("SaveGame Error!"));
	}
}