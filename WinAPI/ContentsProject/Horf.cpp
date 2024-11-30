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
	/* 탐색범위 : */ SetDetectRange({ 500 , 300 });
	/* 발사속도 : */ SetShootingSpeed(350.0f);
	/* 쿨타임   : */ SetCooldown(4.0f);

	BodyScale = { 112, 112 };

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 45, 45 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_Body);
	BodyCollision->SetCollisionType(ECollisionType::Circle);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "monster_029_horf.png", { 1, 1 }, 0.15f, false);
	BodyRenderer->CreateAnimation("Attack", "monster_029_horf.png", { 0, 2, 3 }, 0.2f, false);
	BodyRenderer->CreateAnimation("Aggressive", "monster_029_horf.png", { 0, 0 }, 0.2f, false);
	BodyRenderer->SetComponentLocation({ 0, -10 });
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

	TimeEventer.PushEvent(0.5f, std::bind(&AMonster::SwitchIsAttacking, this));
	TimeEventer.PushEvent(0.65f, [this]() {BodyRenderer->ChangeAnimation("Aggressive"); });

	TearDir = GetDirectionToPlayer();
	FVector2D TearPos = { GetActorLocation().iX(),  GetActorLocation().iY() };

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

	// 진동 속도와 크기 설정
	const float OscillationSpeed = 5.0f;       // 진동 속도
	const float OscillationMagnitude = 2.0f;  // 진동 크기 (최대 이동 거리)

	float OffsetX = std::sin(_DeltaTime * OscillationSpeed) * OscillationMagnitude;
	float OffsetY = std::cos(_DeltaTime * OscillationSpeed) * OscillationMagnitude;

	// 기존 위치를 중심으로 새로운 위치 계산
	FVector2D OriginalPosition = CurPos - ParentRoom->GetActorLocation(); // 기본 위치
	FVector2D NewPosition = { OriginalPosition.X + OffsetX, OriginalPosition.Y + OffsetY };

	BodyRenderer->SetComponentLocation(NewPosition);
}

