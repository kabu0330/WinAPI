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

AAttackFly::AAttackFly()
{
	/* �̸�     : */ SetName("AttackFly");
	/* ü��     : */ SetHp(5);
	/* ���ݷ�   : */ SetAtt(1);
	/* �̵��ӵ� : */ SetMoveSpeed(10);
	/* �̵��ð� : */ SetMoveDuration(1.5f);
	/* �����ð� : */ SetMoveCooldown(2.0f);
	/* Ž������ : */ SetDetectRange({ 300 , 300 });


	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ 25, 25 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Monster_FlyingBody);
	BodyCollision->SetCollisionType(ECollisionType::CirCle);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Idle", "Fly.png", {1, 2, 1, 2, 1, 2, 3, 2, 1, 2, 1, 2, 1, 0} , 0.15f);
	BodyRenderer->CreateAnimation("Death", "Fly.png", 4, 14, 0.08f, false);
	BodyRenderer->SetComponentScale({ 256, 256 });
	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetOrder(ERenderOrder::Monster);

	DetectCollision = CreateDefaultSubObject<U2DCollision>();
	DetectCollision->SetComponentScale(GetDetectRange());
	DetectCollision->SetCollisionGroup(ECollisionGroup::Monster_DetectInRange);
	DetectCollision->SetCollisionType(ECollisionType::CirCle);
	DetectCollision->SetActive(true);
}

void AAttackFly::BeginPlay()
{
	Super::BeginPlay();
	AMonster::BeginPlay();
}

void AAttackFly::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AMonster::Tick(_DeltaTime);
}

void AAttackFly::Death(float _DeltaTime)
{
	// 1. �浹ü(�ٵ� + Ž��) ����
	if (nullptr != BodyCollision)
	{
		CollisionDestroy();
	}

	// 3. ���� �����
	if (nullptr == BodyRenderer)
	{
		Destroy();
		return;
	}

	BodyRenderer->ChangeAnimation("Death");
	BodyRenderer->SetComponentScale({ 256, 256 });

	// 2. ���� ����
	RendererDestroy();
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

void AAttackFly::FollowBoss(float _DeltaTime)
{
	if (nullptr == ParentBoss)
	{
		return;
	}

	FVector2D ParentBossPos = ParentBoss->GetActorLocation();

	Angle += OrbitSpeed * _DeltaTime;

	if (Angle >= static_cast<float>(2 * std::numbers::pi))
	{
		Angle -= static_cast<float>(2 * std::numbers::pi);
	}

	FVector2D NewPosition = FVector2D::ZERO;
	NewPosition.X = ParentBossPos.X + OrbitRadius * cos(Angle);
	NewPosition.Y = ParentBossPos.Y + OrbitRadius * sin(Angle);

	SetActorLocation(NewPosition);

}

AAttackFly::~AAttackFly()
{
}
