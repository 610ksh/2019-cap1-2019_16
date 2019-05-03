// Fill out your copyright notice in the Description page of Project Settings.

#include "GSAnimInstance.h"

// ������
UGSAnimInstance::UGSAnimInstance()
{
	// ���� ĳ������ �ӵ� �ʱ�ȭ
	CurrentPawnSpeed = 0.0f;
	// ���� ���� ���´� false
	IsInAir2 = false;

	// ��Ÿ�� �ּ� ���� ���. ��Ÿ�ִ� Object ������.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack_Montage1(TEXT(
		"/Game/SH/Animations/Greystone_Skeleton_Montage1.Greystone_Skeleton_Montage1"));
	// ������ ã������
	if (Attack_Montage1.Succeeded())
	{
		// ��Ÿ�� �ּ� ���� ���
		AttackMontage = Attack_Montage1.Object;
	}
}

// �Ϲ� Tick�� ������ ��ġ. �ǽð����� �ִϸ��̼� �ý����� ƽ���� ���� ������ ���� �ӷ� ���� ����
void UGSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	// AnimInstance�� �ִ� NativeUpdateAnimation�� ������ �Ѱ���.
	Super::NativeUpdateAnimation(DeltaSeconds);

	// APawn ��ü ������, ���� �ش��ϴ� �� �ִ��ν��Ͻ��� �����ϴ� ���� ������ �ѱ�.
	auto Pawn = TryGetPawnOwner();

	// �ִ� �ν��Ͻ��� Pawn ������ ����� ���� �Դٸ�,
	if (::IsValid(Pawn))
	{
		// ���� �ӵ��� float���� ��ȯ.
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		// �ӽ� ĳ���� ������ ����� �츮�� �����ϰ� �ִ� ĳ���� Ŭ���� ������ �Ѱ��ְ�
		auto Character = Cast<ACharacter>(Pawn);
		// ĳ���� Ŭ������ ��ȿ�ϴٸ�,
		if (Character)
		{
			// IsFalling �������� Ȯ���Ѵ�.
			IsInAir2 = Character->GetMovementComponent()->IsFalling();
		}
	}
}

// ���� ��Ÿ�� ��� �Լ�
void UGSAnimInstance::PlayAttackMontage()
{
	/// ��������Ʈ�� ���� ������ ���۰� ���ᰡ �����Ǳ� ������ IsPlaying���� Ȯ������ �ʾƵ� ��.
	// AttackMontage�� ����ǰ� ���� �ʴٸ�,
	//if (!Montage_IsPlaying(AttackMontage))
	//{
	//	// ��Ÿ�� �����Ŵ (�����ų ��Ÿ�� ����, �ӵ�)
	//	Montage_Play(AttackMontage, 1.0f);
	//}
	
	// ��ϵ� ��Ÿ�� ���.
	Montage_Play(AttackMontage, 1.0f);
}

// ��Ÿ�ֿ��� ���� ���� �������� �Ѿ�� �Լ�
void UGSAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	// Montage_JumpToSection�� �̹� �ִ��ν��Ͻ����� ����. (���� ���� �ѹ�, ��Ÿ�� ����)
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

// ��Ƽ���� �Լ�.
void UGSAnimInstance::AnimNotify_AttackHitCheck()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackHitCheck"));
	OnAttackHitCheck.Broadcast();
}

void UGSAnimInstance::AnimNotify_NextAttackCheck()
{
	UE_LOG(LogTemp, Warning, TEXT("NextAttackCheck"));
	OnNextAttackCheck.Broadcast();
}

FName UGSAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack %d\n"),Section));
}
