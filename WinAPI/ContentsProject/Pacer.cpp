#include "PreCompile.h"
#include "Pacer.h"

#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineRandom.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "ContentsEnum.h"

APacer::APacer()
{
	/* 이름     : */ SetName("Pacer");
	/* 체력     : */ SetHp(10);
	/* 공격력   : */ SetAtt(1);
	/* 이동속도 : */ SetMoveSpeed(100);
	/* 이동시간 : */ SetMoveDuration(2.0f);
	/* 정지시간 : */ SetMoveCooldown(1.0f);
	/* 탐색범위 : */ SetDetectRange({ 0 , 0 });

	float FrameSpeed = 0.13f;

	std::string SpriteName = "monster_000_bodies03.png";

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Left", SpriteName, 36, 45, FrameSpeed);
	BodyRenderer->CreateAnimation("Right", SpriteName, { 14, 15, 18, 19, 20, 21, 24, 25, 26, 27 }, FrameSpeed);
	BodyRenderer->CreateAnimation("Down", SpriteName, {0, 1, 2, 3, 6, 7, 8, 9, 12, 13}, FrameSpeed);
	BodyRenderer->CreateAnimation("Up", SpriteName, { 13, 12, 9, 8, 7, 6, 3, 2, 1, 0 }, FrameSpeed);
	BodyRenderer->CreateAnimation("Idle", SpriteName, 0, 0, FrameSpeed);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale({ 64, 64 });
	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monster);

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 30, 30 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_Body);
	BodyCollision->SetCollisionType(ECollisionType::Rect);

}

APacer::~APacer()
{
}

void APacer::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();
}

void APacer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);
}

void APacer::CurStateAnimation(float _DeltaTime)
{
	if (true == IsDeath())
	{
		return;
	}
	if (nullptr == BodyRenderer)
	{
		return;
	}
	if (true == IsAttack)
	{
		return;
	}

	std::string Left = "Left";
	std::string Right = "Right";
	std::string Up = "Up";
	std::string Down = "Down";

	switch (State)
	{
	case MonsterState::LEFT:
		BodyRenderer->ChangeAnimation(Left);
		break;
	case MonsterState::RIGHT:
		BodyRenderer->ChangeAnimation(Right);
		break;
	case MonsterState::UP:
		BodyRenderer->ChangeAnimation(Up);
		break;
	case MonsterState::DOWN:
		BodyRenderer->ChangeAnimation(Down);
		break;
	case MonsterState::ATTCK_LEFT:
	case MonsterState::ATTCK_RIGHT:
	case MonsterState::ATTCK_UP:
	case MonsterState::ATTCK_DOWN:
	case MonsterState::RUNAWAY_LEFT:
	case MonsterState::RUNAWAY_RIGHT:
	case MonsterState::RUNAWAY_UP:
	case MonsterState::RUNAWAY_DOWN:
	case MonsterState::RUNAWAY_NONE:
	case MonsterState::ATTCK_NONE:
	case MonsterState::NONE:
	case MonsterState::MAX:
	default:
		BodyRenderer->ChangeAnimation("Idle");
		break;
	}
}
