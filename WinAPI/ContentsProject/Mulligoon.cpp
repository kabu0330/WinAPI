#include "PreCompile.h"
#include "Mulligoon.h"

#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineRandom.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "ContentsEnum.h"
#include "Global.h"
#include "Room.h"
#include "DecalObject.h"

AMulligoon::AMulligoon()
{
	/* 이름     : */ SetName("Mulligoon");
	/* 체력     : */ SetHp(15);
	/* 공격력   : */ SetAtt(1);
	/* 이동속도 : */ SetMoveSpeed(90);
	/* 이동시간 : */ SetMoveDuration(2.0f);
	/* 정지시간 : */ SetMoveCooldown(0.0f);
	/* 탐색범위 : */ SetDetectRange({ 550 , 400 });
	/* 발사속도 : */ SetShootingSpeed(450.0f);
	/* 쿨타임   : */ SetCooldown(0.0f);

	State = MonsterState::NONE;

	float FaceFrameSpeed = 0.2f;
	float FrameSpeed = 0.13f;
	BodyScale = { 64, 64 };

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Left", "MulligoonBody_Left.png", { 18, 19, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 }, FrameSpeed);
	BodyRenderer->CreateAnimation("Right", "MulligoonBody.png", {18, 19, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44}, FrameSpeed);
	BodyRenderer->CreateAnimation("Down", "MulligoonBody.png", {0, 1, 2, 3, 9, 10, 11, 16, 9}, FrameSpeed);
	BodyRenderer->CreateAnimation("Up", "MulligoonBody.png", { 9, 16, 11, 10, 9, 3, 2, 1, 0 }, FrameSpeed);
	BodyRenderer->CreateAnimation("Idle", "MulligoonBody.png", 0, 0, FrameSpeed);
	BodyRenderer->SetComponentLocation({ 0, 15 });
	BodyRenderer->SetComponentScale(BodyScale);
	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monster);


	HeadRenderer = CreateDefaultSubObject<USpriteRenderer>();
	HeadRenderer->CreateAnimation("Left", "monster_059_muligoon_left.png", 0, 2, FaceFrameSpeed);
	HeadRenderer->CreateAnimation("Right", "monster_059_muligoon_right.png", 0, 2, FaceFrameSpeed);
	HeadRenderer->CreateAnimation("RunAway_Left", "monster_059_muligoon_left.png", 3, 5, FaceFrameSpeed);
	HeadRenderer->CreateAnimation("RunAway_Right", "monster_059_muligoon_right.png", 3, 5, FaceFrameSpeed);
	HeadRenderer->CreateAnimation("Attack_Left", "monster_059_muligoon_left.png", { 6, 7, 8 }, { 1.0f, 0.1f, 0.1f }, false);
	HeadRenderer->CreateAnimation("Attack_Right", "monster_059_muligoon_right.png", { 6, 7, 8 }, { 1.0f, 0.1f, 0.1f }, false);
	HeadRenderer->SetComponentLocation({ 1, Global::PlayerHeadOffset.iY() + 36 });
	HeadRenderer->SetComponentScale({ 256, 256 });
	HeadRenderer->ChangeAnimation("Left");
	HeadRenderer->SetOrder(ERenderOrder::Monster);

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, -15 });
	BodyCollision->SetComponentScale({ 30, 50 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_Body);
	BodyCollision->SetCollisionType(ECollisionType::Rect);

	DetectCollision = CreateDefaultSubObject<U2DCollision>();
	DetectCollision->SetComponentScale(GetDetectRange());
	DetectCollision->SetCollisionGroup(ECollisionGroup::Monster_DetectInRange);
	DetectCollision->SetCollisionType(ECollisionType::Rect);
	DetectCollision->SetActive(true);

	UniversalCollision = CreateDefaultSubObject<U2DCollision>();
	UniversalCollision->SetComponentLocation({ 0, 0 });
	UniversalCollision->SetComponentScale({ 250, 250 });
	UniversalCollision->SetCollisionGroup(ECollisionGroup::Item_UniversalHit);
	UniversalCollision->SetCollisionType(ECollisionType::Rect);
	UniversalCollision->SetActive(false);

}

AMulligoon::~AMulligoon()
{
}

void AMulligoon::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();
}

void AMulligoon::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);

	CheckDirection();
	Oscillation(_DeltaTime);
	Attack(_DeltaTime);

	SoundTimeElapsed += _DeltaTime;
}

void AMulligoon::Move(float _DeltaTime)
{
	if (true == IsHit)
	{
		return;
	}
	if (nullptr == BodyRenderer)
	{
		return;
	}

	PlayerDetected = IsPlayerNearby();
	if (true == PlayerDetected) // 플레이어를 추적하지 않는 중이면
	{
		return;
	}

	MoveElapsedTime += _DeltaTime;
	if (MoveElapsedTime > MoveDuration) // 일정 시간 이동하면 리턴
	{
		if (MoveElapsedTime > MoveCooldown) // 멈춘 뒤 일정 시간이 흐르면 다시 이동
		{
			Direction = GetRandomDir(); // 플레이어가 탐색 범위에 없다면 랜덤 이동
			MoveElapsedTime = 0.0f;
			return;
		}
		return;
	}
	SetMoveSpeed(100);

	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);
}

void AMulligoon::ChasePlayer(float _DeltaTime)
{
	PlayerDetected = IsPlayerNearby();
	if (false == PlayerDetected)
	{
		return;
	}

	ChaseMove(_DeltaTime);
}

void AMulligoon::ChaseMove(float _DeltaTime)
{
	if (true == IsDeath())
	{
		return;
	}

	SetMoveSpeed(150);

	Force = FVector2D::ZERO;
	Direction = -1 * GetDirectionToPlayer();
	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);

	RunAwayTimeElapsed += _DeltaTime;

	RunAwaySound(_DeltaTime);
}

void AMulligoon::RunAwaySound(float _DeltaTime)
{
	if (SoundTimeElapsed > SoundDuration)
	{
		SoundTimeElapsed = 0.0f;
		IsPlaySound = true;
	}

	if (false == IsPlaySound)
	{
		return;
	}

	Sound = UEngineSound::Play("scared_whimper.wav");
	IsPlaySound = false;
}

void AMulligoon::Attack(float _DeltaTime)
{
	if (RunAwayTimeElapsed < RunAwayDuration)
	{
		return;
	}
	if (nullptr == BodyRenderer)
	{
		return;
	}

	Invincibility = true; // 무적
	IsAttack = true;

	SetMoveSpeed(0);
	HeadRenderer->ChangeAnimation("Attack_Right");
	HeadRenderer->SetComponentScale({ 224, 224 });
	BodyRenderer->ChangeAnimation("Idle");

	RunAwayTimeElapsed = 0.0f;
	TimeEventer.PushEvent(1.2f, [this, _DeltaTime]() {
		SummonBombLogic();
		SummonBombRender();
		TearFire();
		Hp = 0; });

}

void AMulligoon::TearFire()
{
	FVector2D TearPos = FVector2D::ZERO;
	TearDir = GetDirectionToPlayer();
	FVector2D OffsetPos = TearDir * 30.0f;
	for (int i = 0; i < 11; i++)
	{
		FVector2D TearPos = { GetActorLocation().iX() + OffsetPos.iX(),  GetActorLocation().iY() + OffsetPos.iY()};
		TearDir = UEngineMath::AngleToRadian<FVector2D>(TearDir, 30.0f * i);
		Tear = GetWorld()->SpawnActor<ABloodTear>();
		Tear->Fire(TearPos, TearDir, ShootingSpeed, Att);
	}
}

void AMulligoon::SummonBombLogic()
{
	UniversalCollision->SetActive(true);
	UniversalCollision->Destroy(0.1f);
}

void AMulligoon::SummonBombRender()
{
	Sound = UEngineSound::Play("explosion_weak1.wav");

	FVector2D UniversalScale = { 150, 150 };
	USpriteRenderer* ExplosionEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	ExplosionEffectRenderer->CreateAnimation("Explosion", "Explosion.png", 0, 11, 0.1f, false);
	ExplosionEffectRenderer->SetComponentLocation({ 0, 50 });
	ExplosionEffectRenderer->SetComponentScale(UniversalScale * 5.5f);
	ExplosionEffectRenderer->SetOrder(ERenderOrder::ItemEffect);
	ExplosionEffectRenderer->ChangeAnimation("Explosion");
}

void AMulligoon::Death(float _DeltaTime)
{
	if (false == IsAttack)
	{
		SummonBombLogic();
		SummonBombRender();
		IsAttack = true;
	}

	// 1. 충돌체(바디 + 탐색) 끄고
	if (nullptr != BodyCollision)
	{
		Sound = UEngineSound::Play("death_burst_small.wav");
		Sound = UEngineSound::Play("meat_impact.wav");

		BloodEffectRenderer->SetActive(true);
		CreateGib();
		CollisionDestroy();
	}

	// 3. 액터 지우고
	if (nullptr == BodyRenderer)
	{
		ParentRoom->RemoveMonster(this);
		Destroy();
		return;
	}

	BloodEffectRenderer->ChangeAnimation("DeathEffect");


	if (true == BloodEffectRenderer->IsCurAnimationEnd())
	{
		BodyRenderer->Destroy();
		BodyRenderer = nullptr;
		BloodEffectRenderer->Destroy(0.1f);
		return;
	}

	BodyRenderer->SetActive(false);
	HeadRenderer->SetActive(false);
}

int AMulligoon::ApplyDamaged(AActor* _Monster, int _PlayerAtt, FVector2D _Dir)
{
	AMonster* Monster = dynamic_cast<AMonster*>(_Monster);
	if (nullptr == Monster)
	{
		return 0;
	}
	else if (true == Monster->IsInvincible()) // 무적이면 리턴
	{
		return Hp;
	}

	if (true == Monster->IsDeath())
	{
		return 0;
	}

	DamagedEffectRenderer->SetActive(true);
	DamagedEffectRenderer->ChangeAnimation("DamagedEffect");
	BeginBlinkEffect();

	IsHit = true;



	TimeEventer.PushEvent(KnockbackDuration, std::bind(&AMonster::SwitchIsHit, this));

	Hp -= _PlayerAtt;
	if (Hp < 0)
	{
		Hp = 0;
	}
	return Hp;
}

void AMulligoon::Oscillation(float _DeltaTime)
{
	if (true == IsDeath())
	{
		return;
	}
	if (nullptr == BodyRenderer)
	{
		return;
	}

	// 진동 속도와 강도 설정
	const float OscillationSpeed = 1000.0f; // 진동 속도
	const float OscillationMagnitude = 10.0f; // 진동 크기

	// 사인 함수를 이용해 진동 계산
	float Oscillation = std::sin(_DeltaTime * OscillationSpeed) * OscillationMagnitude;

	// 업데이트
	FVector2D BodyRendererScale = BodyScale;
	FVector2D HeadRendererScale = { 256, 256 };
	FVector2D NewBodyScale = { BodyRendererScale.X + Oscillation,BodyRendererScale.Y + Oscillation };
	FVector2D NewHeadScale = { HeadRendererScale.X + Oscillation,HeadRendererScale.Y + Oscillation };
	BodyRenderer->SetComponentScale(NewBodyScale);
	HeadRenderer->SetComponentScale(NewHeadScale);
}

void AMulligoon::CurStateAnimation(float _DeltaTime)
{
	if (true == IsDeath())
	{
		return;
	}
	if (nullptr == BodyRenderer)
	{
		return;
	}
	if (true == IsAttack)
	{
		return;
	}

	std::string Left = "Left";
	std::string Right = "Right";
	std::string Up = "Up";
	std::string Down = "Down";
	std::string RunAway = "RunAway_";
	std::string Attack = "Attack_";

	switch (State)
	{
	case MonsterState::LEFT:
		HeadRenderer->ChangeAnimation(Left);
		BodyRenderer->ChangeAnimation(Left);
		break;
	case MonsterState::RIGHT:
		HeadRenderer->ChangeAnimation(Right);
		BodyRenderer->ChangeAnimation(Right);
		break;
	case MonsterState::UP:
		BodyRenderer->ChangeAnimation(Up);
		break;
	case MonsterState::DOWN:
		BodyRenderer->ChangeAnimation(Down);
		break;
	case MonsterState::ATTCK_LEFT:
		HeadRenderer->ChangeAnimation(Attack + Left);
		BodyRenderer->ChangeAnimation(Left);
		break;
	case MonsterState::ATTCK_RIGHT:
		HeadRenderer->ChangeAnimation(Attack + Right);
		BodyRenderer->ChangeAnimation(Right);
		break;
	case MonsterState::ATTCK_UP:
		HeadRenderer->ChangeAnimation(Attack + Left);
		BodyRenderer->ChangeAnimation(Up);
		break;
	case MonsterState::ATTCK_DOWN:
		HeadRenderer->ChangeAnimation(Attack + Left);
		BodyRenderer->ChangeAnimation(Down);
		break;
	case MonsterState::RUNAWAY_LEFT:
		HeadRenderer->ChangeAnimation(RunAway + Left);
		BodyRenderer->ChangeAnimation(Left);
		break;
	case MonsterState::RUNAWAY_RIGHT:
		HeadRenderer->ChangeAnimation(RunAway + Right);
		BodyRenderer->ChangeAnimation(Right);
		break;
	case MonsterState::RUNAWAY_UP:
		HeadRenderer->ChangeAnimation(RunAway + Up);
		BodyRenderer->ChangeAnimation(Up);
		break;
	case MonsterState::RUNAWAY_DOWN:
		HeadRenderer->ChangeAnimation(RunAway + Down);
		BodyRenderer->ChangeAnimation(Down);
		break;
	case MonsterState::RUNAWAY_NONE:
	case MonsterState::ATTCK_NONE:
	case MonsterState::NONE:
	case MonsterState::MAX:
	default:
		HeadRenderer->ChangeAnimation(Left);
		BodyRenderer->ChangeAnimation("Idle");
		break;
	}
}

void AMulligoon::CheckDirection()
{
	if (true == IsDeath())
	{
		return;
	}
	if (nullptr == BodyRenderer)
	{
		return;
	}
	if (true == IsAttack)
	{
		return;
	}

	PlayerDetected = IsPlayerNearby();
	if (true == PlayerDetected)
	{
		return;
	}

	if (0.0f > Direction.X)
	{
		State = MonsterState::LEFT;
	}
	else if (0.0f < Direction.X)
	{
		State = MonsterState::RIGHT;
	}
	else if (0.0f > Direction.Y)
	{
		State = MonsterState::UP;
	}
	else if (0.0f < Direction.Y)
	{
		State = MonsterState::DOWN;
	}
}