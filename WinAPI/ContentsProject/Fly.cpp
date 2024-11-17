#include "PreCompile.h"
#include "Fly.h"

#include <EngineBase/EngineMath.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "ContentsEnum.h"
#include "Global.h"

AFly::AFly()
{
	/* 이름     : */ SetName("AttackFly");
	/* 체력     : */ SetHp(3);
	/* 공격력   : */ SetAtt(0);
	/* 이동속도 : */ SetMoveSpeed(0);
	/* 이동시간 : */ SetMoveDuration(1.0f);
	/* 정지시간 : */ SetMoveCooldown(2.0f);
	/* 탐색범위 : */ SetDetectRange({ 0 , 0});


	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 25, 25 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_FlyingBodyNonCollision);
	BodyCollision->SetCollisionType(ECollisionType::CirCle);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "Fly002.png", { 0, 1, 0, 1, 0, 9 }, 0.15f);
	BodyRenderer->CreateAnimation("Death", "Fly.png", 4, 14, 0.08f, false);
	BodyRenderer->SetComponentScale({ 64, 64 });
	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monster);
}

AFly::~AFly()
{
}

void AFly::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();
}

void AFly::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);
}

void AFly::Death(float _DeltaTime)
{
	// 1. 충돌체(바디 + 탐색) 끄고
	if (nullptr != BodyCollision)
	{
		CollisionDestroy();
	}

	// 3. 액터 지우고
	if (nullptr == BodyRenderer)
	{
		Destroy();
		return;
	}
	
	BodyRenderer->ChangeAnimation("Death");
	BodyRenderer->SetComponentScale({ 256, 256 });

	// 2. 렌더 끄고
	RendererDestroy();
}


