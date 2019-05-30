// Fill out your copyright notice in the Description page of Project Settings.

#include "GSWeapon.h"

// Sets default values
AGSWeapon::AGSWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// ���̷�Ż �޽� ������Ʈ ����
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	// ��Ʈ ������Ʈ ����
	RootComponent = Weapon;
	// ���� �ּ� ������ ã�Ƽ� ���
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT(
		"/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));
	// ���� �ش� ���� �ּ� ������ �� �����Դٸ�
	if (SK_WEAPON.Succeeded())
	{
		// ������Ʈ�� �ּ��� ����
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}
	// ���̷�Ż �޽� ������Ʈ�� �浹 ������ NoCollision���� ����
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
	
	AttackRange = 150.0f;
	// ������ �ּ� -2.5
	AttackDamageMin = -2.5f;
	// ������ �ִ�
	AttackDamageMax = 10.0f;
	AttackModifierMin = 0.85f;
	AttackModifierMax = 1.25f;
}

float AGSWeapon::GetAttackRange() const
{
	return AttackRange;
}

float AGSWeapon::GetAttackDamage() const
{
	return AttackDamage;
}

float AGSWeapon::GetAttackModifier() const
{
	return AttackModifier;
}

// Called when the game starts or when spawned
void AGSWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// ���� ���� ������ ���� ������ ����
	AttackDamage = FMath::RandRange(AttackDamageMin, AttackDamageMax);
	// ���� ���� ������ ���ݷ� ����ġ ����
	AttackModifier = FMath::RandRange(AttackModifierMin, AttackModifierMax);
	ABLOG(Warning, TEXT("Weapon Damage : %f, Modifier : %f"), AttackDamage, AttackModifier);
}

// Called every frame
void AGSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

