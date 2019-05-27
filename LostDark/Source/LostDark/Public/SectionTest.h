// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "GameFramework/Actor.h"
#include "SectionTest.generated.h"

UCLASS()
class LOSTDARK_API ASectionTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASectionTest();
	// �����Ϳ��� ������ �Ӽ��̳� Ʈ������ ������ ����ɶ� �ڵ� ȣ��Ǵ� �Լ�
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	enum class ESectionState : uint8
	{
		READY=0,
		BATTLE,
		COMPLETE
	};

	// ������Ʈ ����
	void SetState(ESectionState NewState);
	// ���� ���´� Ready
	ESectionState CurrentState = ESectionState::READY;
	// �� ������ �Լ�
	void OperateGates(bool bOpen = true);

	// �߾� �ڽ� Ʈ������ ��������Ʈ
	UFUNCTION()
		void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent*OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	// �� �ڽ� Ʈ������ ��������Ʈ
	UFUNCTION()
		void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	// NPC �����Լ�
	void OnNPCSpawn();

	// �������� ���� ���Ͱ� ��������
	UFUNCTION()
		void OnKeyAIDestroyed(AActor* DestroyedActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// ��� �޽�
	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* Mesh;
	// ���� Ʈ����
	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
		UBoxComponent* Trigger;

	// �� �޽�
	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
		TArray<UStaticMeshComponent*> GateMeshes;
	// �� Ʈ����
	UPROPERTY(VisibleAnywhere, Category = Trigger, Meta = (AllowPrivateAccess = true))
		TArray<UBoxComponent*> GateTriggers;

	// Battle �������� ���� ����
	UPROPERTY(EditAnywhere, Category = State, Meta = (AllowPrivateAccess = true))
		bool bNoBattle;
	
	// NPC ���� Ÿ��
	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
		float NPCSpawnTime;
	// ������ ���� Ÿ��
	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
		float ItemBoxSpawnTime;

	FTimerHandle SpawnNPCTimerHandle = {};
	FTimerHandle SpawnItemBoxTimerHandle = {};
};
