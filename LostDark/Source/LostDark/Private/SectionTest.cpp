// Fill out your copyright notice in the Description page of Project Settings.

#include "SectionTest.h"
// NPC ������ ���� �ʿ�
#include "LostDarkCharacter.h"
// ������ ������ ���� �ʿ�
#include "SH_ItemBox.h"

// Sets default values
ASectionTest::ASectionTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	FString AssetPath = TEXT("/Game/DefaultLevel/SM_SQUARE.SM_SQUARE");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUARE(*AssetPath);
	if (SM_SQUARE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_SQUARE.Object);
	}
	else
	{
		ABLOG(Error, TEXT("Failed to load staticmesh asset. : %s"), *AssetPath);
	}
	// Ʈ���� ������Ʈ ����
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	// �⺻ �ڽ� ������Ʈ �ش� ũ��� ���� (������)
	Trigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	// �ݸ��� Preset�� �̸��� ���ƾ���. �׷��� �����
	Trigger->SetCollisionProfileName(TEXT("LDTrigger"));

	// �߾� �ڽ� Ʈ���� ��������Ʈ �Լ� ���ε�
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASectionTest::OnTriggerBeginOverlap);

	/* Gate �ּ� */

	FString GateAssetPath = TEXT("/Game/DefaultLevel/SM_GATE.SM_GATE");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(*GateAssetPath);
	if (!SM_GATE.Succeeded())
	{
		ABLOG(Error, TEXT("Failed to load staticmesh asset. :%s"), *GateAssetPath);
	}

	static FName GateSockets[] = { {TEXT("+XGate")},{TEXT("-XGate")},{TEXT("+YGate")},{TEXT("-YGate")} };

	for (FName GateSocket : GateSockets)
	{
		ABCHECK(Mesh->DoesSocketExist(GateSocket));
		// ���ο� ����ƽ�޽� ������Ʈ �߰�
		UStaticMeshComponent* NewGate = CreateDefaultSubobject<UStaticMeshComponent>(*GateSocket.ToString());
		ABLOG(Error, TEXT("Success GateMeshs :%s"), *GateSocket.ToString());
		// ���ο� ����Ʈ�� �޽� �߰�
		NewGate->SetStaticMesh(SM_GATE.Object);
		// ���ϸ� �־���.
		NewGate->SetupAttachment(RootComponent, GateSocket);
		// ����Ʈ �޽� ��ġ ����
		NewGate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
		// TArray�� �޽� ������Ʈ ���� �־���.
		GateMeshes.Add(NewGate);
		//ABLOG(Error, TEXT("Success GateMeshs :%s"), *GateAssetPath);

		// �� Ʈ���� ������Ʈ ����
		UBoxComponent* NewGateTrigger = CreateDefaultSubobject<UBoxComponent>(*GateSocket.ToString().Append(TEXT("Trigger")));
		NewGateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		NewGateTrigger->SetupAttachment(RootComponent, GateSocket);
		NewGateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		// �ݸ��� Preset ����
		NewGateTrigger->SetCollisionProfileName(TEXT("LDTrigger"));
		// TArray�� ����
		GateTriggers.Add(NewGateTrigger);

		// �� �ڽ� Ʈ���� ��������Ʈ �Լ� ���ε�
		NewGateTrigger->OnComponentBeginOverlap.AddDynamic(this, &ASectionTest::OnGateTriggerBeginOverlap);
		// �� �� �ڽ� Ʈ���ſ� �±� ����
		NewGateTrigger->ComponentTags.Add(GateSocket);
	}

	// �⺻������ false
	bNoBattle = false;

	// �ʱⰪ ����
	NPCSpawnTime = 2.0f;
	ItemBoxSpawnTime = 5.0f;
}
// �����Ϳ��� ������ �Ӽ��̳� Ʈ������ ������ ����ɶ� �ڵ� ȣ��Ǵ� �Լ�
void ASectionTest::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

// Called when the game starts or when spawned
void ASectionTest::BeginPlay()
{
	Super::BeginPlay();
	// ���� ���۽� bNoBattle ���� ���� ����
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

void ASectionTest::SetState(ESectionState NewState)
{
	switch (NewState)
	{
	case ASectionTest::ESectionState::READY:
		// �߾� Ʈ���� �ݸ��� ����
		Trigger->SetCollisionProfileName(TEXT("LDTrigger"));
		// �� Ʈ���� �ݸ��� �缳��
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}
		// ó������ ���� �� ��������.
		OperateGates(true);
		break;
	case ASectionTest::ESectionState::BATTLE:
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}
		OperateGates(false);

		// AI Ÿ�̸� ������ ���������� ���ε�
		GetWorld()->GetTimerManager().SetTimer(SpawnNPCTimerHandle, FTimerDelegate::CreateUObject(this, &ASectionTest::OnNPCSpawn), NPCSpawnTime, false);
		// ������ ���� Ÿ�̸� ������ ���������͸� ���ٷ� ���ε�
		GetWorld()->GetTimerManager().SetTimer(SpawnItemBoxTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
			// ���̰� ���� �ݰ� 600cm���� ������ x,y��
			FVector2D RandXY = FMath::RandPointInCircle(600.0f);
			// ��� ������ �߽���ġ + ���� 30
			GetWorld()->SpawnActor<ASH_ItemBox>(GetActorLocation() + FVector(RandXY, 30.0f), FRotator::ZeroRotator);
		}), ItemBoxSpawnTime, false);

		break;
	case ASectionTest::ESectionState::COMPLETE:
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("LDTrigger"));
		}
		OperateGates(true);
		break;
	default:
		break;
	}
	// ������� ����
	CurrentState = NewState;
}
// �� ������ �Լ�
void ASectionTest::OperateGates(bool bOpen)
{
	for (UStaticMeshComponent* Gate : GateMeshes)
	{
		Gate->SetRelativeRotation(bOpen ? FRotator(0.0f, -90.0f, 0.0f) : FRotator::ZeroRotator);
	}
}

// �߾� �ڽ� Ʈ������ ��������Ʈ
void ASectionTest::OnTriggerBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// ���� ���� ������Ʈ��
	if (CurrentState == ESectionState::READY)
	{
		// ��Ʋ ������Ʈ�� ��ȯ
		SetState(ESectionState::BATTLE);
	}
}

// �� �ڽ� Ʈ������ ��������Ʈ
void ASectionTest::OnGateTriggerBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// �ش� Ʈ������ �±װ� 1���� �ƴϸ� ����ó��.
	ABCHECK(OverlappedComponent->ComponentTags.Num() == 1);
	// ���� ���� �±� �̸��� ����
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	// ���� �̸��� �޾ƿ�. ���ʿ��� 2��° �̸����� �̸��� ������.
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	ABLOG(Warning, TEXT("Success GateMeshs :%s"), *SocketName.ToString());
	// ����ó��
	if (!Mesh->DoesSocketExist(SocketName))
		return;

	// ������ ������ ���� ��ġ���� �޾ƿ�.
	FVector NewLocation = Mesh->GetSocketLocation(SocketName);

	// 
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllObjects);

	// �������� ����Ʈ���̽�
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		ObjectQueryParam,
		FCollisionShape::MakeSphere(775.0f),
		CollisionQueryParam
	);

	// ������ ���ٸ� ���� ������. GetWorld()->SpawnActor<>();
	if (!bResult)
	{
		auto NewSection = GetWorld()->SpawnActor<ASectionTest>(NewLocation, FRotator::ZeroRotator);
	}
	// ������ �ִٸ� �̹� ���������
	else
	{
		ABLOG(Warning, TEXT("New section area is not empty."));
	}
}

void ASectionTest::OnNPCSpawn()
{
	// �� �߾� + ���� 88�� ����. ĳ���� ���̰� 176�� �����ѵ�?
	GetWorld()->SpawnActor<ALostDarkCharacter>(GetActorLocation() + FVector::UpVector*88.0f, FRotator::ZeroRotator);
}

// Called every frame
void ASectionTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}