#include "PreCompile.h"
#include "Host.h"

#include <EngineBase/EngineMath.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "ContentsEnum.h"
#include "Global.h"

AHost::AHost()
{
	/* �̸�     : */ SetName("Host");
	/* ü��     : */ SetHp(12);
	/* ���ݷ�   : */ SetAtt(1);
	/* �̵��ӵ� : */ SetMoveSpeed(0);
	/* �̵��ð� : */ SetMoveDuration(0.0f);
	/* �����ð� : */ SetMoveCooldown(0.0f);
	/* Ž������ : */ SetDetectRange({ 660 , 660 });
	/* �߻�ӵ� : */ SetShootingSpeed(400.0f);
	/* ��Ÿ��   : */ SetCooldown(3.0f);

	SetInvincible(false);

	HeadCollisionScale = FVector2D(25, 20);

	HeadCollision = CreateDefaultSubObject<U2DCollision>();
	HeadCollision->SetComponentLocation({ 0, 10});
	HeadCollision->SetComponentScale(HeadCollisionScale);
	HeadCollision->SetCollisionGroup(ECollisionGroup::Monster_Barrier);
	HeadCollision->SetCollisionType(ECollisionType::Rect);

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 8 });
	BodyCollision->SetComponentScale({ 20, 35 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_Body);
	BodyCollision->SetCollisionType(ECollisionType::Rect);
	BodyCollision->SetActive(false);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "Host.png", { 0, 0 }, 0.1f, false);
	BodyRenderer->CreateAnimation("Attack", "Host.png", 0, 1, 0.1f, false);
	BodyRenderer->CreateAnimation("Hold", "Host.png", 2, 2, 0.1f, false);
	BodyRenderer->SetComponentLocation({ 0, -33 });
	BodyRenderer->SetComponentScale({ 64, 128 });
	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monster);

	DetectCollision = CreateDefaultSubObject<U2DCollision>();
	DetectCollision->SetComponentScale(GetDetectRange());
	DetectCollision->SetCollisionGroup(ECollisionGroup::Monster_DetectInRange);
	DetectCollision->SetCollisionType(ECollisionType::Circle);
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
	
	if (false == IsAttacking()) // ���� ���� �ƴϸ� �ٵ�� ������
	{
		if (nullptr == BodyCollision)
		{
			return;
		}
		BodyCollision->SetActive(false);
	}
	if (true == IsDeath())
	{
		return;
	}
	// ��������
}

void AHost::Attack(float _DeltaTime)
{
	if (nullptr == BodyRenderer)
	{
		return;
	}

	CooldownElapsed += _DeltaTime;
	if (CooldownElapsed < Cooldown)
	{
		return;
	}
	IsAttack = true;
	BodyRenderer->ChangeAnimation("Attack");
	BodyCollision->SetActive(true);
	HeadCollision->SetActive(false);

	FVector2D DefenseCollisionOffsetPos = FVector2D(0, -30);
	HeadCollision->SetComponentLocation(DefenseCollisionOffsetPos);
	HeadCollision->SetComponentScale(HeadCollisionScale);

	float PreAttackDelay = Cooldown + 0.5f;
	if (PreAttackDelay > CooldownElapsed)
	{
		return;
	}

	Sound = UEngineSound::Play("blood_fire_3.wav");

	FireTripleShotForward();

	CooldownElapsed = 0.0f;
	TimeEventer.PushEvent(3.0f, std::bind(&AHost::ChangeIdle, this));
}

void AHost::FireTripleShotForward()
{
	FVector2D TearPos = { GetActorLocation().iX(),  GetActorLocation().iY() };
	TearDir = GetDirectionToPlayer();
	Tear = GetWorld()->SpawnActor<ABloodTear>();
	Tear->Fire(TearPos, TearDir, ShootingSpeed, Att);
	
	FVector2D DownAngle = UEngineMath::AngleToRadian<FVector2D>(TearDir, 45.0f);
	Tear = GetWorld()->SpawnActor<ABloodTear>();
	Tear->Fire(TearPos, DownAngle, ShootingSpeed, Att);

	FVector2D UpAngle = UEngineMath::AngleToRadian<FVector2D>(TearDir, -45.0f);
	Tear = GetWorld()->SpawnActor<ABloodTear>();
	Tear->Fire(TearPos, UpAngle, ShootingSpeed, Att);
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
	IsAttack = false;
	BodyRenderer->ChangeAnimation("Idle");
	BodyCollision->SetActive(false);
	HeadCollision->SetActive(true);
	HeadCollision->SetComponentLocation({ 0, 10 });
	HeadCollision->SetComponentScale(HeadCollisionScale);
	
}

