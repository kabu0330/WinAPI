#include "PreCompile.h"
#include "Fly.h"

#include <EngineBase/EngineMath.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "ContentsEnum.h"
#include "Global.h"

AFly::AFly()
{
	/* �̸�     : */ SetName("AttackFly");
	/* ü��     : */ SetHp(3);
	/* ���ݷ�   : */ SetAtt(0);
	/* �̵��ӵ� : */ SetMoveSpeed(0);
	/* �̵��ð� : */ SetMoveDuration(1.0f);
	/* �����ð� : */ SetMoveCooldown(2.0f);
	/* Ž������ : */ SetDetectRange({ 0 , 0});


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
	// 1. �浹ü(�ٵ� + Ž��) ����
	if (nullptr != BodyCollision)
	{
		CollisionDestroy();
	}

	// 3. ���� �����
	if (nullptr == BodyRenderer)
	{
		Destroy();
		return;
	}
	
	BodyRenderer->ChangeAnimation("Death");
	BodyRenderer->SetComponentScale({ 256, 256 });

	// 2. ���� ����
	RendererDestroy();
}


