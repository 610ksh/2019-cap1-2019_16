// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "GameFramework/Actor.h"
#include "SH_ItemBox.generated.h"

UCLASS()
class LOSTDARK_API ASH_ItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASH_ItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// PostinitializeComponents
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �ڽ� �ݸ��� ������Ʈ(Ʈ����)
	UPROPERTY(VisibleAnywhere, Category = Box)
		UBoxComponent* Trigger;

	// �ڽ� ����ƽ�޽�
	UPROPERTY(VisibleAnywhere, Category = Box)
		UStaticMeshComponent* Box;

	// ��ƼŬ�ý��� ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystemComponent* Effect;

	// Ŭ���� ������ ������ ������ ������ ���ڿ� ��ӹ��� Ŭ������� ����� ������Ŵ. (TSubclassof)
	UPROPERTY(EditInstanceOnly, Category = Box)
		TSubclassOf<class AGSWeapon> WeaponItemClass;

private:
	// Overlap �̺�Ʈ �߻��Ҷ� ȣ���� ��� �Լ�. ���ε��� ����. ���� ������ �Ʒ��� �ݵ�� ���ƾ߸���.
	UFUNCTION()
		void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// ��ƼŬ �ý����� OnSystemFinished ��������Ʈ�� ���ε��� ��� ��� �Լ�.
	UFUNCTION()
		void OnEffectFinished(class UParticleSystemComponent* PSystem);
};
