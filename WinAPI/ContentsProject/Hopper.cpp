#include "PreCompile.h"
#include "Hopper.h"

#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineRandom.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "ContentsEnum.h"

AHopper::AHopper()
{
	/* 이름     : */ SetName("Hopper");
	/* 체력     : */ SetHp(9);
	/* 공격력   : */ SetAtt(1);
	/* 이동속도 : */ SetMoveSpeed(180);
	/* 이동시간 : */ SetMoveDuration(0.8f);
	/* 정지시간 : */ SetMoveCooldown(1.4f);
	/* 탐색범위 : */ SetDetectRange({ 0 , 0 });

	
	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 30, 30 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_FlyingBody);
	BodyCollision->SetCollisionType(ECollisionType::Rect);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Hopper_Idle", "Hopper.png", { 0, 0 }, 0.1f);
	BodyRenderer->CreateAnimation("Hopper_Move", "Hopper.png", { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 1, 0}, 0.12f, false);
	BodyRenderer->SetComponentLocation({ -7, -15 });
	BodyRenderer->SetComponentScale({ 64, 64 });
	BodyRenderer->ChangeAnimation("Hopper_Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monster);
}

AHopper::~AHopper()
{
}

void AHopper::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();
}

void AHopper::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);

	if (true == IsDeath())
	{
		return;
	}
	// 공격패턴
}

void AHopper::Move(float _DeltaTime)
{
	if (nullptr == BodyRenderer)
	{
		return;
	}

	MoveElapsedTime += _DeltaTime;
	if (MoveElapsedTime > MoveDuration) // 일정 시간 이동하면 리턴
	{
		if (true == BodyRenderer->IsCurAnimationEnd())
		{
			BodyRenderer->ChangeAnimation("Hopper_Idle");
			return;
		}
		if (MoveElapsedTime > MoveCooldown) // 멈춘 뒤 일정 시간이 흐르면 다시 이동
		{
			RandomSpeed();
			BodyRenderer->ChangeAnimation("Hopper_Move");
			Direction = GetRandomDir();
			MoveElapsedTime = 0.0f;
			return;
		}
		return;
	}

	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);
}

void AHopper::RandomSpeed()
{
	UEngineRandom Random;
	Random.SetSeed(time(NULL));
	float RandomSpeed = Random.RandomFloat(150.0f, 210.0f);
	SetMoveSpeed(RandomSpeed);
}
