// Fill out your copyright notice in the Description page of Project Settings.

#include "LostDark/SH/Public/GSWeapon.h"

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
	
}

// Called when the game starts or when spawned
void AGSWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

