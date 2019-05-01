// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LostDark.h"
#include "Animation/AnimInstance.h"
#include "GSAnimInstance.generated.h"

/**
 * GS = GreyStone
 */
UCLASS()
class LOSTDARK_API UGSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// ������
	UGSAnimInstance();

private:
	// ���� ĳ���� �ӵ� ������Ƽ(�������)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;

};
