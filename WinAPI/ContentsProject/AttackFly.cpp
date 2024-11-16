#include "PreCompile.h"
#include "AttackFly.h"

#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineRandom.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "PlayGameMode.h"

#include "Global.h"
#include "ContentsEnum.h"

AAttackFly::AAttackFly()
{
	/* 이름     : */ SetName("AttackFly");
	/* 체력     : */ SetHp(5);
	/* 공격력   : */ SetAtt(1);
	/* 이동속도 : */ SetMoveSpeed(40);
	/* 이동시간 : */ SetMoveDuration(1.5f);
	/* 정지시간 : */ SetMoveCooldown(2.0f);
	/* 탐색범위 : */ SetDetectRange({ 400 ,400 });


	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 25, 25 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_FlyingBody);
	BodyCollision->SetCollisionType(ECollisionType::CirCle);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "Fly.png", 1, 2, 0.1f);
	BodyRenderer->CreateAnimation("Death", "Fly.png", 4, 14, 0.08f, false);
	BodyRenderer->SetComponentScale({ 256, 256 });
	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monster);

	DetectCollision = CreateDefaultSubObject<U2DCollision>();
	DetectCollision->SetComponentScale(GetDetectRange());
	DetectCollision->SetCollisionGroup(ECollisionGroup::Monster_DetectInRange);
	DetectCollision->SetCollisionType(ECollisionType::CirCle);
	DetectCollision->SetActive(true);
}

void AAttackFly::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();
}

void AAttackFly::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);
}

void AAttackFly::Death(float _DeltaTime)
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

AAttackFly::~AAttackFly()
{
}
