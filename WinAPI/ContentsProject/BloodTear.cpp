#include "PreCompile.h"
#include "BloodTear.h"

#include <EngineCore/2DCollision.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include <EngineCore/Actor.h>
#include "Player.h"
#include "Monster.h"

ABloodTear::ABloodTear()
{
	SetActorLocation(FVector2D::ZERO);

	TearCollision = CreateDefaultSubObject<U2DCollision>();
	TearCollision->SetComponentLocation({ 0, 0 });
	TearCollision->SetComponentScale({ 25, 25 });
	TearCollision->SetCollisionGroup(ECollisionGroup::MONSTER_ATTACK);
	TearCollision->SetCollisionType(ECollisionType::CirCle);

	TearEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	TearEffectRenderer->CreateAnimation("BloodTear_Normal", "effect_bloodtear.png", 0, 0, 0, false);
	TearEffectRenderer->CreateAnimation("BloodTear_Attack", "effect_bloodtear.png", 1, 15, 0.05f, false);
	TearEffectRenderer->SetComponentScale({ 96, 96 }); // 64, 64
	TearEffectRenderer->SetOrder(ERenderOrder::TEAR);
	TearEffectRenderer->ChangeAnimation("BloodTear_Normal");
	TearEffectRenderer->SetActive(true);

	DebugOn();
}

void ABloodTear::Fire(FVector2D _StartPos, FVector2D _Dir, float _Speed, float _Att)
{
	TearEffectRenderer->SetActive(true);
	SetActorLocation(_StartPos);
	Dir = _Dir;
	ActorAtt = _Att;
}

void ABloodTear::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	TimeElapesd += _DeltaTime;
	if (Duration < TimeElapesd)
	{
		TriggerExplosion(_DeltaTime);
	}
}

void ABloodTear::TriggerExplosion(float _DeltaTime)
{
	if (nullptr != TearCollision)
	{
		TearCollision->Destroy();
		TearCollision = nullptr;
		Dir = FVector2D::ZERO;
		TearEffectRenderer->ChangeAnimation("BloodTear_Attack");
		SetActorLocation(GetActorLocation());

		if (nullptr != TearEffectRenderer)
		{
			Destroy(0.4f);
		}
	}
}

void ABloodTear::BeginPlay()
{
	Super::BeginPlay();
}

ABloodTear::~ABloodTear()
{
}