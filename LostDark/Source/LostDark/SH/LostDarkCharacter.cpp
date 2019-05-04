// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LostDarkCharacter.h"
#include "LostDarkPlayerController.h"
#include "GSAnimInstance.h"

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
	SetControlMode(EControlMode::BackView);

	// �ܺ� �޽� �ʱ� ��ġ��, ���� �𸮾� ��ǥ�迡 �°� �缳�� (���� 88��ŭ ������, ȸ�� ������ 90�� ȸ��)
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	// �ܺ� �޽� ������ ������ ��������. �ؿܺθ޽ô� ������Ʈ�̴� => FObjectFinder
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Greystone(TEXT(
		"/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone"));

	// �޽ð� ��ȿ�ϴٸ�,
	if (Greystone.Succeeded())
	{
		// ���� ĳ���Ϳ� �޽� �ʱ�ȭ(����). �޽ô� ������Ʈ�̹Ƿ� Object�� ������ �ѱ�.
		GetMesh()->SetSkeletalMesh(Greystone.Object);
	}

	// �ִϸ��̼��� �ִϸ��̼� �������Ʈ�� ó���ϱ� ���� �ִϸ��̼� ��� �ɼ� �缳��. �ؾִϸ��̼��� Mesh�� ���� �����ϱ� ������ GetMesh()���� ������.
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// �ִϸ��̼� �ν��Ͻ� ������ ������ ��������. �ؾִϸ��̼� �ν��Ͻ��� Ŭ�����̴� => FClassFinder, �������� _C�� ���������.
	static ConstructorHelpers::FClassFinder<UAnimInstance> Greyston_Anim(TEXT(
		"/Game/SH/Animations/GS_AnimBlueprint.GS_AnimBlueprint_C"));

	// �ִ��ν��Ͻ��� ��ȿ�ϴٸ�,
	if (Greyston_Anim.Succeeded())
	{
		// ���� ĳ������ �ִ��ν��Ͻ��� �ܺ� �ִ��ν��Ͻ��� �ʱ�ȭ(����)
		GetMesh()->SetAnimInstanceClass(Greyston_Anim.Class);
	}

	// ���� ���濡�� ī�޶� ����� ����Ǵ� �ӵ� ����
	BoomLengthSpeed = 3.0f;
	BoomRotationSpeed = 10.0f;

	// �����ӵ� ����. Z�� �ӵ� ����. 
	GetCharacterMovement()->JumpZVelocity = 450.0f;

	// �ʱ� ���ݻ��´� false
	IsAttacking = false;

	// �ִ� �޺� ī��Ʈ ��
	MaxCombo = 3;
	// �����Ҷ� ���� ȣ���ϰ� ������.
	AttackEndComboState();
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
		// SizeSquard() ���߿� ã�ƺ���
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

// ��� ������Ʈ�� �ʱ�ȭ�Ǹ� �Ҹ������� �Լ�. ������ BeginPlay�Լ��ε�. ���̰� �ִµ� ����.
void ALostDarkCharacter::PostInitializeComponents()
{
	// �θ�� ȣ��
	Super::PostInitializeComponents();
	// GS �ִ��ν��Ͻ� ���� ��������
	GSAnim = Cast<UGSAnimInstance>(GetMesh()->GetAnimInstance());
	// GS �ִ��ν��Ͻ��� �������� ���ߴٸ� ����ó���� �Լ���ȯ
	ABCHECK(nullptr != GSAnim);
	// ��Ÿ�� ����� ������, �ڵ����� OnAttackMontageEnded ��� �Լ��� ȣ���ϴ°���. AnimInstance->OnMontageEnded�� �̹� ������.
	GSAnim->OnMontageEnded.AddDynamic(this, &ALostDarkCharacter::OnAttackMontageEnded);
	UE_LOG(LogTemp, Warning, TEXT("PostInitializeComponents, AddDynamic"));

	// ��Ƽ���� ��ȣ�� ������ �Ϸ� ����. void�� ��ȯ
	GSAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		// OnNextAttackCheck ��ε�ĳ��Ʈ�� �ߵ����� Log�� ����.
		UE_LOG(LogTemp, Warning, TEXT("OnNextAttackCheck Lambda Called"));
		//ABLOG(Warning, TEXT("OnNextAttackCheck"));
		// ���� �޺��� ������ �ϰ�
		CanNextCombo = false;
		// �޺� �Է��� ���Դٸ� // NextAttackCheck ��Ƽ���� ��ε�ĳ��Ʈ �������� ������ 2���̻� ���ȴٸ�
		if (IsComboInputOn)
		{
			// �޺� ���� �Լ� ȣ�� (�޺� ī��Ʈ ����)
			AttackStartComboState();
			// ���� ���� �޺��� �ҷ���
			GSAnim->JumpToAttackMontageSection(CurrentCombo);
			UE_LOG(LogTemp, Warning, TEXT("Called JumpTo Function, Combo : %d"),CurrentCombo);
		}
	});
}

// Input ���� (������ Input�� ���� Tick�Լ�)
void ALostDarkCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// BindAction ����. EInputEvent::IE_Pressed, Released �Լ��� �ִ�. ������ ����.
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &ALostDarkCharacter::ViewChange);

	// Jump ����. Jump�� �⺻������ Character Ŭ������ �̹� ����Ǿ� �־, �θ��� �Լ��� ���ε� ��Ű�� ��.
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ALostDarkCharacter::Jump);

	// Attack, Mouse X ���ε��ϱ�.
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ALostDarkCharacter::Attack);

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

// ���콺 ��Ŭ��, Attack ����. ��Ÿ�ָ� ����� ���� �ִϸ��̼��� �����.
void ALostDarkCharacter::Attack()
{
	// ���� ��Ÿ�� ������̶��
	if (IsAttacking)
	{
		// ���� ���� Comboī��Ʈ�� 1~3 ���̰� �ƴ϶�� ��ȯ (����ó��)
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		// �������ε�, ���� �޺��� ���� �ִٸ�,
		if (CanNextCombo)
		{
			// �޺��� �־��ٴ� �ֹ��� �־���.
			IsComboInputOn = true;
			//UE_LOG(LogTemp, Warning, TEXT("Can NextCombo : %d"),CurrentCombo);
		}
	}
	// �������� �ƴ϶��,
	else
	{
		// ���� ���� Combo�� 0�� �ƴϸ� ��ȯ
		ABCHECK(CurrentCombo == 0);
		// �޺� ������Ʈ ȣ��. �޺� ī��Ʈ ���� 1��
		AttackStartComboState();
		// �ִ��ν��Ͻ� ��Ÿ�� �Լ� ȣ��. ���� �ִϸ��̼� ���
		GSAnim->PlayAttackMontage();
		// ���� �������� ���� �޺� ī��Ʈ�� �ѱ�.
		GSAnim->JumpToAttackMontageSection(CurrentCombo);
		// ���� ���� ���� ����
		IsAttacking = true;
	}
	//// �ִ� �ν��Ͻ� ������. Mesh�� �޷��ִ� �ִ��ν��Ͻ��� UGS�� ����ȯ�� ���.
	//auto AnimInstance = Cast<UGSAnimInstance>(GetMesh()->GetAnimInstance());
	//// Ȥ�� �ִ� �ν��Ͻ��� ���� �������� (exception handler)
	//if (nullptr == AnimInstance) return;
	//// �ִ� �ν��Ͻ��� �ִ� ��Ÿ�� �Լ��� �����Ŵ.
	//AnimInstance->PlayAttackMontage();

	//IsAttacking = true;
}

// ��������Ʈ�� ������ �Լ�. ��Ÿ�ְ� ������ ������, �ڵ����� �Ҹ�����.
void ALostDarkCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruped)
{
	// IsAttacking�� �ƴ϶�� �ٷ� ���� ���� ���� return.
	ABCHECK(IsAttacking);
	// CurrentCombo�� 0���� ũ�� ������ return.
	ABCHECK(CurrentCombo > 0);
	// ������� ��Ÿ�ְ� ������ IsAttacking ������ �ٽ� false�� �ʱ�ȭ.
	IsAttacking = false;
	// ��Ÿ�ְ� ������ ȣ��
	AttackEndComboState();
	// ��Ÿ�� ������ Log ����ֱ�
	UE_LOG(LogTemp, Warning, TEXT("MontageEnded"));
}

// ������ �����Ҷ� ���� �Ӽ� �����ϴ� �Լ�. Combo ī��Ʈ�� ������Ŵ
void ALostDarkCharacter::AttackStartComboState()
{
	// ���� �޺��� �Ѿ �� ����.
	CanNextCombo = true;
	// ���� �޺��� ���� ����. (����)
	IsComboInputOn = false;
	// ���� CurrentCombo�� 0~2 ���� �ƴ϶�� ���� ���� ����ϰ� ��ȯ
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	/*
		Clamp : �ּ� �ִ밪 ���� �Լ�. (���� �� , �ּ�, �ִ�) �۰ų� �ִ밪���� ũ��, �ش簪�� ������.
		(1,3)�� �ִ���. CurrentCombo�� 3������ ������ �ȵ�. ����� ���ʴ� 0�� ���ͼ� 1�̵�. �׷��� ù��° �޺� �ִϸ��̼� 1�� �����°���
	*/
	/// ���� �޺� ī��Ʈ�� �����ϴ� �ڵ���.
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

// ������ ������ �� ����� �Լ�
void ALostDarkCharacter::AttackEndComboState()
{
	// �޺� �Է� ���θ� �ٽ� false�� �ٲ���
	IsComboInputOn = false;
	// ���� �޺��� ���°� ���� (�ִ�)
	CanNextCombo = false;
	// ���� �޺��� �ٽ� 0���� �ʱ�ȭ
	CurrentCombo = 0;
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