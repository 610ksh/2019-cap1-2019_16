// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "LostDark.h"
#include "GameFramework/Character.h"
#include "LostDarkCharacter.generated.h"

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
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// ���� ��ȯ �Լ�
	void SetControlMode(int32 ControlMode);

private:
	// Called for forwards/backward input
	void MoveForward(float Value);
	// Called for side to side input
	void MoveRight(float Value);

	// �Ʒ� �Լ��� Pawn���� �⺻������ �����ϴ� �Լ��� �־ �װ� �̿��ϸ��.
	//void LookUp(float Value);
	//void Turn(float Value);

// ����ŷ�� �ִ°� ���� �Լ�. ������������?
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};

