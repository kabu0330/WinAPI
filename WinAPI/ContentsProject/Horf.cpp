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
	/* �̸�     : */ SetName("Horf");
	/* ü��     : */ SetHp(10);
	/* ���ݷ�   : */ SetAtt(1);
	/* �̵��ӵ� : */ SetMoveSpeed(0);
	/* �̵��ð� : */ SetMoveDuration(0.0f);
	/* �����ð� : */ SetMoveCooldown(0.0f);
	/* Ž������ : */ SetDetectRange({ 600 , 300 });
	/* �߻�ӵ� : */ SetShootingSpeed(350.0f);
	/* ��Ÿ��   : */ SetCooldown(4.0f);

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

	// ���� �ӵ��� ���� ����
	const float OscillationSpeed = 1500.0f; // ���� �ӵ�
	const float OscillationMagnitude = 4.0f; // ���� ũ��

	// ���� �Լ��� �̿��� ���� ���
	float Oscillation = std::sin(_DeltaTime * OscillationSpeed) * OscillationMagnitude;

	// ��ġ ������Ʈ
	FVector2D NewScale = { BodyScale.X + Oscillation,  BodyScale.Y + Oscillation };
	BodyRenderer->SetComponentScale(NewScale);
}

