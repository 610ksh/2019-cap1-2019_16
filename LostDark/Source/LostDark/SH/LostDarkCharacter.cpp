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
	
	// ���� �ʱ�ȭ (ī�޶�, SpringArm ����)
	SetControlMode(0);

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

	
}


void ALostDarkCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ALostDarkCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Input
void ALostDarkCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


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
	// �θ� Pawn Ŭ������ �ִ� AddControllerYawInput �Լ��� ȣ����
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	// �θ� Pawn Ŭ������ �ִ� AddControllerPitchInput �Լ��� ȣ����
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
}

// ���� ��ȯ �Լ�
void ALostDarkCharacter::SetControlMode(int32 ControlMode)
{
	if (ControlMode == 0)
	{
		// Boom ���� ����.
		CameraBoom->TargetArmLength = 450.0f;
		// Boom ȸ������ (0,0,0) = �÷��̾ �ٶ󺸴� X�� ������ (1,0,0)�� �ǹ���.
		CameraBoom->SetRelativeRotation(FRotator::ZeroRotator);
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
		// ȸ�� �ӵ� ����. (Y ȸ�� �ӵ�, Z ȸ�� �ӵ�, x ȸ���ӵ�) ������ �𸣰���. => �Լ� ������ �ٸ��Ͱ� �ٸ��� �ؼ� �򰥸�. ����!
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // �� Y���� Yaw���� �𸣰���. FRotator �Լ� �������� ������ �ٸ�.
	}
}

// �յ� �̵� �Լ�
void ALostDarkCharacter::MoveForward(float Value)
{
	// Character Ŭ�������� �����ϴ� �̵� �Լ�. (����, �̵�ũ��)
	//AddMovementInput(GetActorForwardVector(), Value);
	// ��Ʈ�� ȸ�� �����κ��� ȸ������� �����, ���ϴ� ���� ���� ������ ĳ���Ͱ� �����̴� �������� �̵��ϰ� �Ѵ�.
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), Value);
}

// �¿� �̵� �Լ�
void ALostDarkCharacter::MoveRight(float Value)
{
	// Character Ŭ�������� �����ϴ� �̵� �Լ�. (����, �̵�ũ��)
	//AddMovementInput(GetActorRightVector(), Value);
	// ��Ʈ�� ȸ�� �����κ��� ȸ������� �����, ���ϴ� ���� ���� ������ ĳ���Ͱ� �����̴� �������� �̵��ϰ� �Ѵ�.
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), Value);
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
