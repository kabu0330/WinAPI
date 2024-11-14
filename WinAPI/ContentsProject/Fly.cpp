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
	/* 이동속도 : */ SetMoveSpeed(50);
	/* 이동시간 : */ SetMoveDuration(1.0f);
	/* 정지시간 : */ SetMoveCooldown(0.0f);
	/* 탐색범위 : */ SetDetectRange({ 0 , 0});


	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 25, 25 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_NonCollision);
	BodyCollision->SetCollisionType(ECollisionType::CirCle);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Fly.Idle", "Fly002.png", 0, 1, 0.1f);
	BodyRenderer->CreateAnimation("Death", "Fly.png", 4, 14, 0.05f, false);
	BodyRenderer->SetComponentScale({ 64, 64 });
	BodyRenderer->ChangeAnimation("Fly.Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monsetr);

	//DetectCollision = CreateDefaultSubObject<U2DCollision>();
	//DetectCollision->SetComponentScale(GetDetectRange());
	//DetectCollision->SetCollisionGroup(ECollisionGroup::Monster_DetectInRange);
	//DetectCollision->SetCollisionType(ECollisionType::CirCle);
	//DetectCollision->SetActive(true);
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


