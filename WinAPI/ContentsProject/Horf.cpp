#include "PreCompile.h"
#include "Horf.h"

#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineRandom.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "ContentsEnum.h"
#include "Room.h"

AHorf::AHorf()
{
	/* 이름     : */ SetName("Horf");
	/* 체력     : */ SetHp(10);
	/* 공격력   : */ SetAtt(1);
	/* 이동속도 : */ SetMoveSpeed(0);
	/* 이동시간 : */ SetMoveDuration(0.0f);
	/* 정지시간 : */ SetMoveCooldown(0.0f);
	/* 탐색범위 : */ SetDetectRange({ 600 , 300 });
	/* 발사속도 : */ SetShootingSpeed(350.0f);
	/* 쿨타임   : */ SetCooldown(4.0f);

	BodyScale = { 108, 108 };

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 48, 48 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_Body);
	BodyCollision->SetCollisionType(ECollisionType::Circle);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "monster_029_horf.png", { 1, 1 }, 0.15f, false);
	BodyRenderer->CreateAnimation("Attack", "monster_029_horf.png", { 0, 2, 3 }, 0.2f, false);
	BodyRenderer->CreateAnimation("Aggressive", "monster_029_horf.png", { 0, 0 }, 1.0f, false);
	BodyRenderer->SetComponentLocation({ 0, 15 });
	BodyRenderer->SetComponentScale(BodyScale);
	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monster);

	DetectCollision = CreateDefaultSubObject<U2DCollision>();
	DetectCollision->SetComponentScale(GetDetectRange());
	DetectCollision->SetCollisionGroup(ECollisionGroup::Monster_DetectInRange);
	DetectCollision->SetCollisionType(ECollisionType::Rect);
	DetectCollision->SetActive(true);
}

AHorf::~AHorf()
{
}

void AHorf::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();

	CurPos = GetActorLocation();
	

}

void AHorf::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);

	Oscillation(_DeltaTime);
}

void AHorf::Attack(float _DeltaTime)
{
	if (true == IsDeath())
	{
		return;
	}

	if (nullptr == BodyRenderer)
	{
		return;
	}

	CooldownElapsed += _DeltaTime;
	if (CooldownElapsed < Cooldown)
	{
		return;
	}

	BodyRenderer->ChangeAnimation("Attack");

	IsAttack = true;

	IsOscillate = true;

	Sound = UEngineSound::Play("shakey kid roar.wav");

	TimeEventer.PushEvent(0.5f, std::bind(&AMonster::SwitchIsAttacking, this));
	TimeEventer.PushEvent(0.65f, [this]() {BodyRenderer->ChangeAnimation("Aggressive"); });

	TearDir = GetDirectionToPlayer();
	FVector2D TearPos = { GetActorLocation().iX(),  GetActorLocation().iY() };
	if (MonsterState::LEFT == State)
	{
		TearPos += FVector2D(-30, 0);
	}
	else
	{
		TearPos += FVector2D(30, 0);
	}

	Tear = GetWorld()->SpawnActor<ABloodTear>();
	Tear->Fire(TearPos, TearDir, ShootingSpeed, Att);
	CooldownElapsed = 0.0f;
}

void AHorf::ChangeAggressiveAnimation()
{
	BodyRenderer->ChangeAnimation("Aggressive");
	IsOscillate = true;
}

void AHorf::Oscillation(float _DeltaTime)
{
	if (true == IsDeath())
	{
		return;
	}
	if (nullptr == BodyRenderer)
	{
		return;
	}
	if (false == IsOscillate)
	{
		return;
	}

	// 진동 속도와 강도 설정
	const float OscillationSpeed = 1500.0f; // 진동 속도
	const float OscillationMagnitude = 4.0f; // 진동 크기

	// 사인 함수를 이용해 진동 계산
	float Oscillation = std::sin(_DeltaTime * OscillationSpeed) * OscillationMagnitude;

	// 위치 업데이트
	FVector2D NewScale = { BodyScale.X + Oscillation,  BodyScale.Y + Oscillation };
	BodyRenderer->SetComponentScale(NewScale);
}

