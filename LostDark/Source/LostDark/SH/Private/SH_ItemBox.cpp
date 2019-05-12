// Fill out your copyright notice in the Description page of Project Settings.

#include "LostDark/SH/Public/SH_ItemBox.h"
#include "LostDark/SH/Public/LostDarkCharacter.h"
#include "LostDark/SH/Public/GSWeapon.h"

// Sets default values
ASH_ItemBox::ASH_ItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Ʈ����, �ڽ�, ����Ʈ(��ƼŬ) ������Ʈ ����
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	// ��Ʈ������Ʈ�� �ڽ� ������Ʈ ���
	RootComponent = Trigger;
	// ��Ʈ�� �ڽ�, ����Ʈ ������Ʈ ���
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	// ��ü �ݸ��� �ڽ� ũ���� ���� ���� �ǹ��Ѵ�. (80 X 82 X 41 �̾���) ���̴� ���� Ű����� ������ �Ѱ���.
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));

	// Box �ּ� ���� ��������
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT(
		"/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (SM_BOX.Succeeded())
	{
		// �ڽ� ������Ʈ�� ������ �ּ� ������ ����(����)
		Box->SetStaticMesh(SM_BOX.Object);
	}

	// �ܺ� ��ƼŬ �ּ��� ������.
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT(
		"/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (P_CHESTOPEN.Succeeded())
	{
		// ����Ʈ ������Ʈ�� �ּ��� ���
		Effect->SetTemplate(P_CHESTOPEN.Object);
		// �ڵ�Ȱ��ȭ ��
		Effect->bAutoActivate = false;
	}

	// �ڽ� ����ƽ�޽��� ��ġ�� ����
	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	// Collisionä�� ���. Object ä���� �̸��� �����ؾ���.
	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	// ����ƽ�޽ÿ� �ڵ����� �پ��ִ� �ݸ����� ���ش�.
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	// ���� Ŭ���� ������ �Ѱ���.
	WeaponItemClass = AGSWeapon::StaticClass();
}

// Called when the game starts or when spawned
void ASH_ItemBox::BeginPlay()
{
	Super::BeginPlay();
}

void ASH_ItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Overlap �ɶ����� �ش��ϴ� �Լ��� �ڵ� ȣ���ϵ��� ���ε���.
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASH_ItemBox::OnCharacterOverlap);
}

// Called every frame
void ASH_ItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Overlap �ɶ����� �ڵ� ȣ��Ǵ� �Լ�. ��������Ʈ. ���ε���.
void ASH_ItemBox::OnCharacterOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABLOG_S(Warning);
	// �浹ó���Ǵ� ����� ������ ���. �츮 �ݸ��� ä�ο��� �ݵ�� �÷��̾ �����ϱ� �̷��� �ϳ��� �����ϰ� �ڵ��ϸ��.
	auto GSCharacter = Cast<ALostDarkCharacter>(OtherActor);
	// ���� �浹�� ����� �÷��̾ �ƴ϶�� ����ó��.
	ABCHECK(nullptr != GSCharacter);

	if (nullptr != GSCharacter && nullptr != WeaponItemClass)
	{
		// �÷��̾ ���⸦ ������ �� �ִٸ� (����̶��)
		if (GSCharacter->CanSetWeapon())
		{
			// ���忡 ���⸦ ������Ŵ. WeaponItemClass�� ��ϵ� ������ �����ؼ�
			auto NewWeapon = GetWorld()->SpawnActor<AGSWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			// �÷��̾ ������Ų ���⸦ ������.
			GSCharacter->SetWeapon(NewWeapon);
			// �̸� ����س��� ����Ʈ Ȱ��ȭ
			Effect->Activate(true);
			// ����ƽ�޽� ��� ���߱�
			Box->SetHiddenInGame(true, true);
			// ������ �ڽ� ��ü �ݸ��� ����
			SetActorEnableCollision(false);
			// ��������Ʈ ����. ��ƼŬ�� ������ ȣ��ǵ��� ���ε���
			Effect->OnSystemFinished.AddDynamic(this, &ASH_ItemBox::OnEffectFinished);
		}
		// �÷��̾ �̹� ���⸦ �����ٸ�,
		else
		{
			// �α� ����.
			ABLOG(Warning, TEXT("%s can't equip weapon currently."), *GSCharacter->GetName());
		}
	}
}

void ASH_ItemBox::OnEffectFinished(UParticleSystemComponent * PSystem)
{
	// �ڽ� ���� (����)
	Destroy();
}

