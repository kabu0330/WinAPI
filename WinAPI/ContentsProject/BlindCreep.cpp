#include "PreCompile.h"
#include "BlindCreep.h"

#include <EngineBase/EngineMath.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>

#include "ContentsEnum.h"
#include "Global.h"

ABlindCreep::ABlindCreep()
{
	/* 이름     : */ SetName("BlindCreep");
	/* 체력     : */ SetHp(10);
	/* 공격력   : */ SetAtt(1);
	/* 이동속도 : */ SetMoveSpeed(300);
	/* 이동시간 : */ SetMoveDuration(0.0f);
	/* 정지시간 : */ SetMoveCooldown(0.0f);
	/* 탐색범위 : */ SetDetectRange({ 1300 , 600 });
	/* 발사속도 : */ SetShootingSpeed(600.0f);
	/* 쿨타임   : */ SetCooldown(3.0f);

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 30, 30 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_Body);
	BodyCollision->SetCollisionType(ECollisionType::Circle);
	BodyCollision->SetActive(false);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "monster_242_blindcreep_champion.png", 4, 7 , 0.15f);
	BodyRenderer->CreateAnimation("Attack", "monster_242_blindcreep_champion.png", 0, 3, 0.2f, false);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale({ 64, 64 });
	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monster);

	DetectCollision = CreateDefaultSubObject<U2DCollision>();
	DetectCollision->SetComponentScale(GetDetectRange());
	DetectCollision->SetCollisionGroup(ECollisionGroup::Monster_DetectInRange);
	DetectCollision->SetCollisionType(ECollisionType::Rect);
	DetectCollision->SetActive(true);
}

ABlindCreep::~ABlindCreep()
{
}

void ABlindCreep::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();
}

void ABlindCreep::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);
}

void ABlindCreep::Attack(float _DeltaTime)
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


	Sound = UEngineSound::Play("spider_spit_roar_0.wav");

	FireTripleShotForward();
	TimeEventer.PushEvent(0.2f, std::bind(&ABlindCreep::FireTripleShotForward, this));
	TimeEventer.PushEvent(0.4f, std::bind(&ABlindCreep::FireTripleShotForward, this));

	CooldownElapsed = 0.0f;
	TimeEventer.PushEvent(0.8f, std::bind(&ABlindCreep::ChangeIdle, this));
}

void ABlindCreep::FireTripleShotForward()
{
	FVector2D TearPos = { GetActorLocation().iX(),  GetActorLocation().iY() };
	TearDir = { GetDirectionToPlayer().X , 0.0f };
	Tear = GetWorld()->SpawnActor<ABloodTear>();
	Tear->Fire(TearPos, TearDir, ShootingSpeed, Att);
}

void ABlindCreep::ChangeIdle()
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
	RandomSpeed();
}

void ABlindCreep::ChaseMove(float _DeltaTime)
{
	if (true == IsHit)
	{
		return;
	}

	Force = FVector2D::ZERO;
	Direction = { 0.0f, GetDirectionToPlayer().Y };
	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);
}

void ABlindCreep::RandomSpeed()
{
	UEngineRandom Random;
	Random.SetSeed(time(nullptr) ^ reinterpret_cast<time_t>(this));
	float RandomSpeed = Random.RandomFloat(100.0f, 550.0f);
	SetMoveSpeed(RandomSpeed);
}

