#include "PreCompile.h"
#include "AttackFly.h"
#include <cmath>

#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineRandom.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/2DCollision.h>
#include "PlayGameMode.h"

#include "Global.h"
#include "ContentsEnum.h"
#include "TheDukeOfFlies.h"
#include "DecalObject.h"

AAttackFly::AAttackFly()
{
	/* 이름     : */ SetName("AttackFly");
	/* 체력     : */ SetHp(Hp);
	/* 공격력   : */ SetAtt(1);
	/* 이동속도 : */ SetMoveSpeed(70); 
	/* 이동시간 : */ SetMoveDuration(1.5f);
	/* 정지시간 : */ SetMoveCooldown(0.0f);
	/* 탐색범위 : */ SetDetectRange({ 480, 480 });

	Hp = 5;
	RendererScale = { 256, 256 };
	CollisionScale = { 25, 25 };

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_FlyingBody);
	BodyCollision->SetCollisionType(ECollisionType::Circle);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "Fly.png", {1, 2, 1, 2, 1, 2, 3, 2, 1, 2, 1, 2, 1, 0} , 0.15f);
	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monster);

	BloodEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BloodEffectRenderer->CreateAnimation("DeathEffect", "Fly.png", 4, 14, 0.08f, false);
	BloodEffectRenderer->SetComponentLocation({ 0, 0 });
	BloodEffectRenderer->SetComponentScale({ 256, 256 });
	BloodEffectRenderer->SetOrder(ERenderOrder::MonsterEffect);
	BloodEffectRenderer->ChangeAnimation("DeathEffect");
	BloodEffectRenderer->SetActive(false);

	DetectCollision = CreateDefaultSubObject<U2DCollision>();
	DetectCollision->SetComponentScale(GetDetectRange());
	DetectCollision->SetCollisionGroup(ECollisionGroup::Monster_DetectInRange);
	DetectCollision->SetCollisionType(ECollisionType::Circle);
	DetectCollision->SetActive(true);
}

void AAttackFly::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();

	// OrbitSpeed 계산
	if (OrbitRadius > 0.0f)
	{
		OrbitSpeed = GetMoveSpeed() / OrbitRadius; // 회전 속도 계산
	}

	BodyCollision->SetComponentScale(CollisionScale);
	BodyRenderer->SetComponentScale(RendererScale);
}

void AAttackFly::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);


	RemoveFly();
}

void AAttackFly::Move(float _DeltaTime)
{
	if (true == IsFollowBoss)
	{
		return;
	}
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

	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);
}

void AAttackFly::ChasePlayer(float _DeltaTime)
{
	PlayerDetected = IsPlayerNearby();
	if (false == PlayerDetected) // 플레이어가 추적 범위 내에 들어왔어?
	{
		if (true == IsFollowBoss) // 보스가 널 소환한 존재야?
		{
			FollowBoss(_DeltaTime); // 플레이어 추적안하면 보스 지키러 가
			return;
		}
		return;
	}

	IsFollowBoss = false;
	ChaseMove(_DeltaTime);
}

void AAttackFly::ChaseMove(float _DeltaTime)
{
	if (true == IsHit)
	{
		return;
	}

	Direction = GetDirectionToPlayer();
	FVector2D MovePos = Direction * Speed * _DeltaTime;
	AddActorLocation(MovePos);
}

void AAttackFly::RemoveFly()
{
	if (true == IsDeath())
	{
		ATheDukeOfFlies* Duke = dynamic_cast<ATheDukeOfFlies*>(ParentBoss);
		if (nullptr == Duke)
		{
			return;
		}

		Duke->GetFliesList().remove(this);
	}
}

void AAttackFly::CreateGib()
{
	int Index = MonsterRandom.RandomInt(0, 26);
	if (Index < 14)
	{
		ARoomObject* Gib = ParentRoom->CreateObject<ADecalObject>(this);
		ADecalObject* Decal = dynamic_cast<ADecalObject*>(Gib);
		if (nullptr != Decal)
		{
			Decal->SetMove(this);
		}

		USpriteRenderer* GibRenderer = Gib->GetBodyRenderer();
		GibRenderer->CreateAnimation("Gib", "effect_030_bloodgibs.png", Index, Index, 0.1f, false);
		GibRenderer->SetComponentScale({ 64, 64 });
		GibRenderer->SetActive(true);
		GibRenderer->SetOrder(ERenderOrder::MonsterDeathDebris);
		GibRenderer->ChangeAnimation("Gib");
	}

	int BloodPoolIndex = MonsterRandom.RandomInt(0, 69);
	if (BloodPoolIndex < 24)
	{
		ARoomObject* BloodPool = ParentRoom->CreateObject<ADecalObject>(this);
		ADecalObject* BloodPoolDecal = dynamic_cast<ADecalObject*>(BloodPool);
		if (nullptr == BloodPoolDecal)
		{
			return;
		}

		USpriteRenderer* BloodPoolRenderer = BloodPool->GetBodyRenderer();
		BloodPoolRenderer->CreateAnimation("BloodPool", "effect_bloodpool.png", BloodPoolIndex, BloodPoolIndex, 0.1f, false);
		BloodPoolRenderer->SetComponentScale({ 256, 256 });
		BloodPoolRenderer->SetActive(true);
		BloodPoolRenderer->SetAlphaFloat(0.5f);
		BloodPoolRenderer->SetOrder(ERenderOrder::Decal);
		BloodPoolRenderer->ChangeAnimation("BloodPool");
	}
}

void AAttackFly::FollowBoss(float _DeltaTime)
{
	if (nullptr == ParentBoss)
	{
		IsFollowBoss = false;
		return;
	}

	FVector2D BossPosition = ParentBoss->GetActorLocation();

	// 각도 업데이트
	Angle -= OrbitSpeed * _DeltaTime;

	if (Angle >= static_cast<float>(2 * std::numbers::pi))
	{
		Angle -= static_cast<float>(2 * std::numbers::pi);
	}

	// 목표 위치 계산 (보스 중심의 원형 궤적)
	FVector2D TargetPosition;
	TargetPosition.X = BossPosition.X + OrbitRadius * cos(Angle);
	TargetPosition.Y = BossPosition.Y + OrbitRadius * sin(Angle);

	// 현재 위치 계산
	FVector2D CurrentPosition = GetActorLocation();
	FVector2D Direction = TargetPosition - CurrentPosition;
	float LengthSquared = Direction.X * Direction.X + Direction.Y * Direction.Y;

	// 목표 위치 근처라면 정확히 위치 고정
	if (LengthSquared <= (GetMoveSpeed() * _DeltaTime) * (GetMoveSpeed() * _DeltaTime))
	{
		SetActorLocation(TargetPosition);
		return; // 위치 고정 후 추가 이동 방지
	}

	// 방향 정규화 후 이동
	if (LengthSquared > 0.0f)
	{
		Direction.X /= sqrt(LengthSquared);
		Direction.Y /= sqrt(LengthSquared);
	}

	FVector2D MoveDelta = Direction * GetMoveSpeed() * _DeltaTime;
	AddActorLocation(MoveDelta);
}

void AAttackFly::SetInitialAngle(float _Angle)
{
	// 초기 각도를 설정
	Angle = _Angle;

	// 보스의 위치를 기준으로 초기 위치 계산
	if (nullptr != ParentBoss)
	{
		FVector2D BossPosition = ParentBoss->GetActorLocation();

		FVector2D NewPosition;
		NewPosition.X = BossPosition.X + OrbitRadius * cos(Angle);
		NewPosition.Y = BossPosition.Y + OrbitRadius * sin(Angle);

		// 부하의 초기 위치 설정
		SetActorLocation(NewPosition);
	}
}

AAttackFly::~AAttackFly()
{
}

