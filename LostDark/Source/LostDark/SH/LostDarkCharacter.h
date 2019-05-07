// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "LostDark.h"
#include "GameFramework/Character.h"
#include "LostDarkCharacter.generated.h"

// AI�� ������ �������� �ߵ��� ��������Ʈ ����. BTTask_Attack Ŭ������ ��������Ʈ ����ϴ� �Լ� ����.
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

/*
	Ŭ���� ������ : Config = ConfigName
	�� Ŭ������ ȯ�漳��(.ini)���Ͽ� �����͸� ������ �� ������ ��Ÿ����.
	�� �����ڴ� �� ������Ƽ�� �ش� �̸��� ȯ�漳�� ���Ͽ� �����ϵ��� �Ѵ�.
	��� �ڽ� Ŭ������ ���ĵǸ�, �ڽ� Ŭ�������� ȯ�漳�� ������ �ٲ� �� �ִ�.
*/
UCLASS(config=Game)
class ALostDarkCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	ALostDarkCharacter();

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	// ������ ���� �ּ� ����
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		USkeletalMeshComponent* Weapon;

	// UI ���� ���漱��. ���Ϳ� UI ������ ������ �� �ִ� UWidgetComponent Ŭ������.
	UPROPERTY(VisibleAnywhere, Category = UI)
		// �ܺ� ���� �������Ʈ Ŭ���� ������ ��� ����
		class UWidgetComponent* HPBarWidget;

	// Attack ����� ����ϴ� �Լ�. AI��Ʈ�ѷ����� ����� ������ �ֵ��� public���� ������
	void Attack();
	// ��������Ʈ ���� ����
	FOnAttackEndDelegate OnAttackEnd;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// ���� ��ȯ ������
	enum class EControlMode
	{
		BackView, //���
		QuaterView, //���ͺ�
		NPC
	};

	// ���� ��ȯ �Լ�
	void SetControlMode(EControlMode NewControlMode);
	// ���� ���� ����
	EControlMode CurrentControlMode = EControlMode::BackView;
	// ���ͺ信�� �չ���(X����)�� ��Ÿ���� �������
	FVector DirectionToMove = FVector::ZeroVector;

	// Boom ����
	float BoomLengthTo = 0.0f;
	// Boom ȸ����
	FRotator BoomRotationTo = FRotator::ZeroRotator;
	// ���� ����� �ش� Boom���� �þ�ų� �پ��� �ӵ�
	float BoomLengthSpeed = 0.0f;
	// ���� ����� �ش� Boom���� ȸ���ϴ� �ӵ�
	float BoomRotationSpeed = 0.0f;

private:
	// Called for forwards/backward input
	void MoveForward(float Value);
	// Called for side to side input
	void MoveRight(float Value);

	// �Ʒ� �Լ��� Pawn���� �⺻������ �����ϴ� �Լ��� �־ �װ� �̿��ϸ��.
	// void LookUp(float Value);
	// void Turn(float Value);

	// ���� ���� �����ϴ� �Լ�
	void ViewChange();

	//// Attack ��� �Լ�
	//void Attack();

	// BP�� ���õ� C++�Լ��� �ݵ�� UFUNCTION ��ũ�θ� ����ؾ��Ѵ�. ���̳��� ��������Ʈ.
	UFUNCTION()
		// ��Ÿ�� ���� ��������Ʈ �Լ�. UAnimMontage�� �������!
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// ������ �����Ҷ� ���� �Ӽ� �����ϴ� �Լ�
	void AttackStartComboState();
	// ������ ������ �� ����� �Լ�
	void AttackEndComboState();
	// ���� �������� �Լ�
	void AttackCheck();

private:
	//�ν��Ͻ� �Ӽ��� �����ִ� ������ ����Ʈ������ �������� VisibleInstanceOnly.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		// ���� ���������� Ȯ���ϴ� ����. 
		bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		// ���� �޺��� �̵� ���� ����
		bool CanNextCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		// �޺� �Է¿���
		bool IsComboInputOn;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		// ���� �������� �޺� ī��Ʈ
		int32 CurrentCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		// �޺� ī��Ʈ �ִ�ġ
		int32 MaxCombo;

	// ���ֻ���� �ִ� �ν��Ͻ� Ŭ������ �����ͷ� �ƿ� ��������. (���漱��)
	UPROPERTY()
		class UGSAnimInstance* GSAnim;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		// ���� ���� ����
		float AttackRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		// ���� �ݰ� (������)
		float AttackRadius;

// ����ŷ�� �ִ°� ���� �Լ�. ������������?
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// ��Ÿ�� ����� ���� ��������Ʈ �Լ� ���� (��� ������Ʈ�� �ʱ�ȭ ������ �Ҹ�����). OnMontageEnded ��������Ʈ�� ���ε��Ѵ�.
	virtual void PostInitializeComponents() override;
	
	
	// AActor�� �ִ� TakeDamage�Լ��� �߰� �����Ѵ�. �����Լ�. �޴ºκп� ���� ������ ���⼭ ������.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	// ���� ������ ���� ���������� �ڵ� ȣ���ϴ� �Լ�
	virtual void PossessedBy(AController* NewController) override;


	// ���� �������� ����
	bool CanSetWeapon();
	// ���⸦ �����ϴ� �Լ�
	void SetWeapon(class AGSWeapon* NewWeapon);
	// ���� Ŭ������ ���� ���漱��
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class AGSWeapon* CurrentWeapon;

	// ĳ���� ���� ������Ʈ ���漱��
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UGSCharacterStatComponent* CharacterStat;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};