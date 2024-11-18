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
	/* �̵��ӵ� : */ SetMoveSpeed(10);
	/* �̵��ð� : */ SetMoveDuration(1.0f);
	/* �����ð� : */ SetMoveCooldown(2.0f);
	/* Ž������ : */ SetDetectRange({ 0 , 0});


	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 25, 25 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_FlyingBodyNonCollision);
	BodyCollision->SetCollisionType(ECollisionType::Circle);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "Fly002.png", { 0, 1, 0, 1, 0, 9 }, 0.15f);
	BodyRenderer->SetComponentScale({ 64, 64 });
	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monster);

	BloodEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BloodEffectRenderer->CreateAnimation("DeathEffect", "Fly.png", 4, 14, 0.08f, false);
	BloodEffectRenderer->SetComponentLocation({ 0, 0 });
	BloodEffectRenderer->SetComponentScale({ 256, 256 });
	BloodEffectRenderer->SetOrder(ERenderOrder::MonsterEffect);
	BloodEffectRenderer->ChangeAnimation("DeathEffect");
	BloodEffectRenderer->SetActive(false);
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



