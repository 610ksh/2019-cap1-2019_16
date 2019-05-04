// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "YJCharacter.generated.h"

UCLASS()
class LOSTDARK_API AYJCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AYJCharacter();

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		int16 HealthPoint = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		int16 MentalPoint = 100;

	UFUNCTION(BlueprintNativeEvent)
		void ReduceHP();
	UFUNCTION(BlueprintNativeEvent)
		void AddHP();
	UFUNCTION(BlueprintNativeEvent)
		void ReduceMP();
	UFUNCTION(BlueprintNativeEvent)
		void AddMP();

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
		QuaterView //���ͺ�
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
	//void LookUp(float Value);
	//void Turn(float Value);

	// ���� ���� �����ϴ� �Լ�
	void ViewChange();

	// ����ŷ�� �ִ°� ���� �Լ�. ������������?
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
