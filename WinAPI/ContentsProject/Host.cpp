#include "PreCompile.h"
#include "Host.h"

#include <EngineBase/EngineMath.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "ContentsEnum.h"
#include "Global.h"

AHost::AHost()
{
	/* 이름     : */ SetName("Host");
	/* 체력     : */ SetHp(12);
	/* 공격력   : */ SetAtt(1);
	/* 이동속도 : */ SetMoveSpeed(0);
	/* 이동시간 : */ SetMoveDuration(0.0f);
	/* 정지시간 : */ SetMoveCooldown(0.0f);
	/* 탐색범위 : */ SetDetectRange({ 500 , 500 });
	/* 발사속도 : */ SetShootingSpeed(400.0f);
	/* 쿨타임   : */ SetCooldown(5.0f);

	SetInvincible(false);

	HeadCollisionScale = FVector2D(35, 30);

	HeadCollision = CreateDefaultSubObject<U2DCollision>();
	HeadCollision->SetComponentLocation({ 0, 10});
	HeadCollision->SetComponentScale(HeadCollisionScale);
	HeadCollision->SetCollisionGroup(ECollisionGroup::Object_Wall);
	HeadCollision->SetCollisionType(ECollisionType::Rect);

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 8 });
	BodyCollision->SetComponentScale({ 20, 35 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_Body);
	BodyCollision->SetCollisionType(ECollisionType::Rect);
	BodyCollision->SetActive(false);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "Host.png", { 0, 0 }, 0.1f);
	BodyRenderer->CreateAnimation("Attack", "Host.png", 0, 1, 0.5f, false);
	BodyRenderer->CreateAnimation("Attack", "Host.png", 0, 1, 0.5f, false);
	BodyRenderer->CreateAnimation("Death", "effect_bloodpoof.png", 0, 9, 0.1f, false);
	BodyRenderer->SetComponentLocation({ 0, -33 });
	BodyRenderer->SetComponentScale({ 64, 128 });
	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monsetr);

	DetectCollision = CreateDefaultSubObject<U2DCollision>();
	DetectCollision->SetComponentScale(GetDetectRange());
	DetectCollision->SetCollisionGroup(ECollisionGroup::Monster_DetectInRange);
	DetectCollision->SetCollisionType(ECollisionType::CirCle);
	DetectCollision->SetActive(true);
}

AHost::~AHost()
{
}

void AHost::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();
}

void AHost::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);
}

void AHost::Attack(float _DeltaTime)
{
	CoolDownElapsed += _DeltaTime;

	if (CoolDownElapsed < Cooldown)
	{
		return;
	}
	BodyRenderer->ChangeAnimation("Attack");
	BodyCollision->SetActive(true);

	FVector2D DefenseCollisionOffsetPos = FVector2D(0, -30);
	HeadCollision->SetComponentLocation(DefenseCollisionOffsetPos);
	HeadCollision->SetComponentScale(HeadCollisionScale);

	float PreAttackDelay = Cooldown + 0.5f;
	if (PreAttackDelay > CoolDownElapsed)
	{
		return;
	}

	TearDir = GetDirectionToPlayer();
	FVector2D TearPos = { GetActorLocation().iX(),  GetActorLocation().iY() };

	Tear = GetWorld()->SpawnActor<ABloodTear>();
	Tear->Fire(TearPos, TearDir, ShootingSpeed, Att);
	CoolDownElapsed = 0.0f;
	TimeEventer.PushEvent(2.5f, std::bind(&AHost::ChangeIdle, this));
}

void AHost::ChangeIdle()
{
	if (nullptr == BodyRenderer)
	{
		return;
	}
	if (nullptr == BodyCollision)
	{
		return;
	}

	BodyRenderer->ChangeAnimation("Idle");
	BodyCollision->SetActive(false);
	HeadCollision->SetComponentLocation({ 0, 10 });
	HeadCollision->SetComponentScale(HeadCollisionScale);
	
}

