// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LostDarkCharacter.h"
#include "LostDarkPlayerController.h"

ALostDarkCharacter::ALostDarkCharacter()
{
	// tick �Լ� Ȱ��ȭ
	PrimaryActorTick.bCanEverTick = true;

	// Boom(=SpingArm) �ʱ�ȭ (�⺻����)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	// Camera �ʱ�ȭ (�⺻����)
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	// Boom ĸ���� ���̱�(���)
	CameraBoom->SetupAttachment(GetCapsuleComponent());
	// Camera�� Boom�� ���̱�(���)
	FollowCamera->SetupAttachment(CameraBoom);

	// ���� ���ͺ�� �ʱ�ȭ (ī�޶�, SpringArm ����)
	SetControlMode(EControlMode::QuaterView);

	// �ܺ� �޽� �ʱ� ��ġ��, ���� �𸮾� ��ǥ�迡 �°� �缳�� (���� 88��ŭ ������, ȸ�� ������ 90�� ȸ��)
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));


	/*
		<���̷�Ż �޽� ����>
	*/
	// �ܺ� �޽� ������ ������ ��������. �ؿܺθ޽ô� ������Ʈ�̴� => FObjectFinder
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Greystone(TEXT(
		"/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone"));

	// �޽ð� ��ȿ�ϴٸ�,
	if (Greystone.Succeeded())
	{
		// ���� ĳ���Ϳ� �޽� �ʱ�ȭ(����). �޽ô� ������Ʈ�̹Ƿ� Object�� ������ �ѱ�.
		GetMesh()->SetSkeletalMesh(Greystone.Object);
	}

	/*
		<�ִϸ��̼� �������Ʈ ����>
	*/
	// �ִϸ��̼��� �ִϸ��̼� �������Ʈ�� ó���ϱ� ���� �ִϸ��̼� ��� �ɼ� �缳��. �ؾִϸ��̼��� Mesh�� ���� �����ϱ� ������ GetMesh()���� ������.
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// �ִϸ��̼� �ν��Ͻ� ������ ������ ��������. �ؾִϸ��̼� �ν��Ͻ��� Ŭ�����̴� => FClassFinder, �������� _C�� ���������.
	static ConstructorHelpers::FClassFinder<UAnimInstance> Greyston_Anim(TEXT(
		"/Game/ParagonGreystone/Characters/Heroes/Greystone/Greystone_AnimBlueprint.Greystone_AnimBlueprint_C"));

	// �ִ��ν��Ͻ��� ��ȿ�ϴٸ�,
	if (Greyston_Anim.Succeeded())
	{
		// ���� ĳ������ �ִ��ν��Ͻ��� �ܺ� �ִ��ν��Ͻ��� �ʱ�ȭ(����)
		GetMesh()->SetAnimInstanceClass(Greyston_Anim.Class);
	}

	BoomLengthSpeed = 3.0f;
	BoomRotationSpeed = 10.0f;

}


void ALostDarkCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ALostDarkCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���� �����Ҷ� ī�޶� �ڸ���� �Լ�. InterpTo�� �̿� (������ : Interpolation)
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, BoomLengthTo, DeltaTime, BoomLengthSpeed);

	switch (CurrentControlMode)
	{
	case ALostDarkCharacter::EControlMode::BackView:
		break;
	case ALostDarkCharacter::EControlMode::QuaterView:
		// ī�޶� �̵�
		CameraBoom->RelativeRotation = FMath::RInterpTo(CameraBoom->RelativeRotation, BoomRotationTo, DeltaTime, BoomRotationSpeed);
		break;
	default:
		break;
	}

	switch (CurrentControlMode)
	{
	case ALostDarkCharacter::EControlMode::BackView:
		break;
	case ALostDarkCharacter::EControlMode::QuaterView:
		// SizeSquard() ??
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			// ĳ���� ��Ʈ�ѷ��� ȸ���� ����. X�� ������ ȸ������� ���ϰ�, �װͿ� ��ġ�ϴ� FRotator(Rotator)���� ����.
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}
		break;
	default:
		break;
	}
}

// Input ���� (������ Input�� ���� Tick�Լ�)
void ALostDarkCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// BindAction ����. EInputEvent::IE_Pressed, Released �Լ��� �ִ�. ������ ����.
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &ALostDarkCharacter::ViewChange);

	/*
		BindAxis(1, 2, 3)
		1 : ������Ʈ ���� Input�� �ִ� BindAxis ���� �̸��� TEXT ���·� �־��ش�.
		2 : ����Ϸ��� Ŭ���� ������ �Ѱ��ش�. �ش� Ŭ�������� ����ϴϱ� this.
		3 : Input�� �������� ȣ���� �Լ��� �ּҸ� �Ѱ��ش�. (�Լ��� �ּҴ� �Լ� �̸�)
		��� : Input�� �ɶ����� 3��° ���ڷ� ���� �Լ��� �ڵ� ȣ����.
	*/
	// Input ������ �ִ� MoveForward ���ε��ϱ�.
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ALostDarkCharacter::MoveForward);
	// Input ������ �ִ� MoveRight ���ε��ϱ�.
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ALostDarkCharacter::MoveRight);

	/// ���� ���ǰͰ� �Ȱ��� ���·� ¥���ϴµ�, ������ �Լ��� �״�� ����غ�.
	// �θ� Pawn Ŭ������ �ִ� AddControllerYawInput �Լ��� ȣ����
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	// �θ� Pawn Ŭ������ �ִ� AddControllerPitchInput �Լ��� ȣ����
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);

}

// ���� ���� ����
void ALostDarkCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case ALostDarkCharacter::EControlMode::BackView:
		GetController()->SetControlRotation(GetActorRotation());
		// BackView������ QuaterView�� ����
		SetControlMode(EControlMode::QuaterView);
		break;
	case ALostDarkCharacter::EControlMode::QuaterView:
		GetController()->SetControlRotation(CameraBoom->RelativeRotation);
		// QuaterView������ BackView�� ����
		SetControlMode(EControlMode::BackView);
		break;
	default:
		break;
	}
}

// ���� ��ȯ �Լ�
void ALostDarkCharacter::SetControlMode(EControlMode NewControlMode)
{
	// ���� ���� ���� �缳��.
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case ALostDarkCharacter::EControlMode::BackView:
		// Boom ���� ����.
		//CameraBoom->TargetArmLength = 450.0f;
		// Boom ȸ������ (0,0,0) = �÷��̾ �ٶ󺸴� X�� ������ (1,0,0)�� �ǹ���.
		//CameraBoom->SetRelativeRotation(FRotator::ZeroRotator);

		// Boom ���� �ʱ�ȭ
		BoomLengthTo = 450.0f;
		// SpringArm�� ȸ�������� �÷��̾� ��Ʈ�ѷ� ȸ������ �����. (= ���콺�� ȸ���ϸ� ī�޶� ���� ������ ȸ����)
		CameraBoom->bUsePawnControlRotation = true;
		// �θ� ������Ʈ(�÷��̾�)���� ȸ������(��ġ,��,��) ��ӹ�����
		CameraBoom->bInheritPitch = true;
		CameraBoom->bInheritRoll = true;
		CameraBoom->bInheritYaw = true;
		// ProbeChannel, ProbeSize�� ����Ͽ� �浹üũ, ī�޶� Ŭ������ �Ͼ�°��� �����Ѵ�. (ī�޶�� �÷��̾� ���̿� �ٸ� ��ü�� �����ϸ�, ĳ���ͷ� ������)
		CameraBoom->bDoCollisionTest = true;
		// �¿� ȸ�� ����. true�� ���, Pawn�� Yaw�� �÷��̾� ��Ʈ�ѷ��� Yaw�� ��Ī�ǵ��� ��. (���콺 ȸ��-> �÷��̾� ��Ʈ�ѷ� -> ���� ȸ���� ��������)
		bUseControllerRotationYaw = false;
		// ĳ���Ͱ� �����̴� �������� ĳ���͸� �ڵ����� ȸ�������ִ� CharacterMovement �Լ���.
		GetCharacterMovement()->bOrientRotationToMovement = true;
		//
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		// ȸ�� �ӵ� ����. (Y ȸ�� �ӵ�, Z ȸ�� �ӵ�, x ȸ���ӵ�) ������ �𸣰���. => �Լ� ������ �ٸ��Ͱ� �ٸ��� �ؼ� �򰥸�. ����!
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // �� Y���� Yaw���� �𸣰���. FRotator �Լ� �������� ������ �ٸ�.
		break;
	case ALostDarkCharacter::EControlMode::QuaterView:
		// ���� 800
		//CameraBoom->TargetArmLength = 800.0f;
		// �Ʒ��� 45�� ȸ��
		//CameraBoom->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

		// Boom ���� �ʱ�ȭ
		BoomLengthTo = 800.0f;
		// Boom ȸ���� �ʱ�ȭ
		BoomRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		// SpringArm�� ȸ�������� �÷��̾� ��Ʈ�ѷ� ȸ������ ���
		CameraBoom->bUsePawnControlRotation = false;
		// �θ� ������Ʈ(�÷��̾�)���� ȸ������(��ġ,��,��) ��ӹ�����
		CameraBoom->bInheritPitch = false;
		CameraBoom->bInheritRoll = false;
		CameraBoom->bInheritYaw = false;
		// ProbeChannel, ProbeSize�� ����Ͽ� �浹üũ, ī�޶� Ŭ������ �Ͼ�°��� �������� ����.
		CameraBoom->bDoCollisionTest = false;
		// Pawn�� Yaw�� �÷��̾� ��Ʈ�ѷ��� Yaw�� ��Ī�ǵ��� ��.
		bUseControllerRotationYaw = false;
		// ĳ���Ͱ� �����̴� �������� ĳ���͸� �ڵ����� ȸ�������ִ� �Լ��� ��.
		GetCharacterMovement()->bOrientRotationToMovement = false;
		// ��� �ڿ������� ȸ���ϴ°��� Ŵ.
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		// ȸ�� �ӵ� ����
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	default:
		break;
	}
}

// �յ� �̵� �Լ�
void ALostDarkCharacter::MoveForward(float Value)
{
	// Character Ŭ�������� �����ϴ� �̵� �Լ�. (����, �̵�ũ��)
	//AddMovementInput(GetActorForwardVector(), Value);

	switch (CurrentControlMode)
	{
	case ALostDarkCharacter::EControlMode::BackView:
		// ��Ʈ�� ȸ�� �����κ��� ȸ������� �����, ���ϴ� ���� ���� ������ ĳ���Ͱ� �����̴� �������� �̵��ϰ� �Ѵ�.
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), Value);
		break;
	case ALostDarkCharacter::EControlMode::QuaterView:
		DirectionToMove.X = Value;
		break;
	default:
		break;
	}
}

// �¿� �̵� �Լ�
void ALostDarkCharacter::MoveRight(float Value)
{
	// Character Ŭ�������� �����ϴ� �̵� �Լ�. (����, �̵�ũ��)
	//AddMovementInput(GetActorRightVector(), Value);

	switch (CurrentControlMode)
	{
	case ALostDarkCharacter::EControlMode::BackView:
		// ��Ʈ�� ȸ�� �����κ��� ȸ������� �����, ���ϴ� ���� ���� ������ ĳ���Ͱ� �����̴� �������� �̵��ϰ� �Ѵ�.
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), Value);
		break;
	case ALostDarkCharacter::EControlMode::QuaterView:
		DirectionToMove.Y = Value;
		break;
	default:
		break;
	}
}

//void ALostDarkCharacter::LookUp(float Value)
//{
//	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), Value);
//}
//
//void ALostDarkCharacter::Turn(float Value)
//{
//	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), Value);
//}