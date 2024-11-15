#include "PreCompile.h"
#include "Pooter.h"

#include <EngineBase/EngineMath.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "ContentsEnum.h"

APooter::APooter()
{
	/* 이름     : */ SetName("Pooter");
	/* 체력     : */ SetHp(14);
	/* 공격력   : */ SetAtt(1);
	/* 이동속도 : */ SetMoveSpeed(1);
	/* 이동시간 : */ SetMoveDuration(0.0f);
	/* 정지시간 : */ SetMoveCooldown(0.0f);
	/* 탐색범위 : */ SetDetectRange({ 600 , 600 });
	/* 발사속도 : */ SetShootingSpeed(300.0f);
	/* 쿨타임   : */ SetCooldown(3.0f);

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 30, 30 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_Body);
	BodyCollision->SetCollisionType(ECollisionType::CirCle);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "Pooter.png", { 0, 1, 14, 15 }, 0.1f);
	BodyRenderer->CreateAnimation("Attack", "Pooter.png", 2, 15, 0.05f, false);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale({ 64, 64 });
	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monster);

	DetectCollision = CreateDefaultSubObject<U2DCollision>();
	DetectCollision->SetComponentScale(GetDetectRange());
	DetectCollision->SetCollisionGroup(ECollisionGroup::Monster_DetectInRange);
	DetectCollision->SetCollisionType(ECollisionType::CirCle);
	DetectCollision->SetActive(true);
}

APooter::~APooter()
{
}

void APooter::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();

}

void APooter::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);

}

void APooter::Attack(float _DeltaTime)
{
	std::string CurAnimationName = BodyRenderer->GetCurAnimationName();

	if ("Attack" == CurAnimationName)
	{
		if (true == BodyRenderer->IsCurAnimationEnd())
		{
			TimeEventer.PushEvent(0.1f, std::bind(&AMonster::ChangeAnimIdle, this));
		}
	}

	CoolDownElapsed += _DeltaTime;
	if (CoolDownElapsed < Cooldown)
	{
		return;
	}

	if ("Idle" == CurAnimationName)
	{
		BodyRenderer->ChangeAnimation("Attack");
	}


	PreparationElapesd += _DeltaTime;
	if (PreparationElapesd < PreparationDuration)
	{
		return;
	}

	TearDir = GetDirectionToPlayer();
	FVector2D TearPos = { GetActorLocation().iX(),  GetActorLocation().iY() + 12 };

	Tear = GetWorld()->SpawnActor<ABloodTear>();
	Tear->Fire(TearPos, TearDir, ShootingSpeed, Att);
	CoolDownElapsed = 0.0f;
	PreparationElapesd = 0.0f;
}


