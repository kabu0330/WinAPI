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
	/* �̸�     : */ SetName("AttackFly");
	/* ü��     : */ SetHp(Hp);
	/* ���ݷ�   : */ SetAtt(1);
	/* �̵��ӵ� : */ SetMoveSpeed(70); 
	/* �̵��ð� : */ SetMoveDuration(1.5f);
	/* �����ð� : */ SetMoveCooldown(0.0f);
	/* Ž������ : */ SetDetectRange({ 480, 480 });

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

	// OrbitSpeed ���
	if (OrbitRadius > 0.0f)
	{
		OrbitSpeed = GetMoveSpeed() / OrbitRadius; // ȸ�� �ӵ� ���
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
	if (true == PlayerDetected) // �÷��̾ �������� �ʴ� ���̸�
	{
		return;
	}

	MoveElapsedTime += _DeltaTime;
	if (MoveElapsedTime > MoveDuration) // ���� �ð� �̵��ϸ� ����
	{
		if (MoveElapsedTime > MoveCooldown) // ���� �� ���� �ð��� �帣�� �ٽ� �̵�
		{
			Direction = GetRandomDir(); // �÷��̾ Ž�� ������ ���ٸ� ���� �̵�
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
	if (false == PlayerDetected) // �÷��̾ ���� ���� ���� ���Ծ�?
	{
		if (true == IsFollowBoss) // ������ �� ��ȯ�� �����?
		{
			FollowBoss(_DeltaTime); // �÷��̾� �������ϸ� ���� ��Ű�� ��
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

	// ���� ������Ʈ
	Angle -= OrbitSpeed * _DeltaTime;

	if (Angle >= static_cast<float>(2 * std::numbers::pi))
	{
		Angle -= static_cast<float>(2 * std::numbers::pi);
	}

	// ��ǥ ��ġ ��� (���� �߽��� ���� ����)
	FVector2D TargetPosition;
	TargetPosition.X = BossPosition.X + OrbitRadius * cos(Angle);
	TargetPosition.Y = BossPosition.Y + OrbitRadius * sin(Angle);

	// ���� ��ġ ���
	FVector2D CurrentPosition = GetActorLocation();
	FVector2D Direction = TargetPosition - CurrentPosition;
	float LengthSquared = Direction.X * Direction.X + Direction.Y * Direction.Y;

	// ��ǥ ��ġ ��ó��� ��Ȯ�� ��ġ ����
	if (LengthSquared <= (GetMoveSpeed() * _DeltaTime) * (GetMoveSpeed() * _DeltaTime))
	{
		SetActorLocation(TargetPosition);
		return; // ��ġ ���� �� �߰� �̵� ����
	}

	// ���� ����ȭ �� �̵�
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
	// �ʱ� ������ ����
	Angle = _Angle;

	// ������ ��ġ�� �������� �ʱ� ��ġ ���
	if (nullptr != ParentBoss)
	{
		FVector2D BossPosition = ParentBoss->GetActorLocation();

		FVector2D NewPosition;
		NewPosition.X = BossPosition.X + OrbitRadius * cos(Angle);
		NewPosition.Y = BossPosition.Y + OrbitRadius * sin(Angle);

		// ������ �ʱ� ��ġ ����
		SetActorLocation(NewPosition);
	}
}

AAttackFly::~AAttackFly()
{
}

