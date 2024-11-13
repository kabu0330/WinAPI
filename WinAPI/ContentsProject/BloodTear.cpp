#include "PreCompile.h"
#include "BloodTear.h"

#include <EngineCore/2DCollision.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include <EngineCore/Actor.h>
#include "Player.h"
#include "Monster.h"
#include "PlayGameMode.h"

ABloodTear::ABloodTear()
{
	SetName("Blood Tear");
	SetActorLocation(FVector2D::ZERO);

	TearCollision = CreateDefaultSubObject<U2DCollision>();
	TearCollision->SetComponentLocation({ 0, 0 });
	TearCollision->SetComponentScale({ 25, 25 });
	TearCollision->SetCollisionGroup(ECollisionGroup::Monster_Attack);
	TearCollision->SetCollisionType(ECollisionType::CirCle);

	TearEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	TearEffectRenderer->CreateAnimation("BloodTear_Normal", "effect_bloodtear.png", 0, 0, 0, false);
	TearEffectRenderer->CreateAnimation("BloodTear_Attack", "effect_bloodtear.png", 1, 15, 0.05f, false);
	TearEffectRenderer->SetComponentScale({ 96, 96 }); // 64, 64
	TearEffectRenderer->SetOrder(ERenderOrder::Tear);
	TearEffectRenderer->ChangeAnimation("BloodTear_Normal");
	TearEffectRenderer->SetActive(true);

	DebugOn();
}

void ABloodTear::Fire(FVector2D _StartPos, FVector2D _Dir, float _Speed, int _Att)
{
	TearEffectRenderer->SetActive(true);
	SetActorLocation(_StartPos);
	Dir = _Dir;
	ActorAtt = _Att;
}

void ABloodTear::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == APlayGameMode::IsGamePaused())
	{
		return;
	}

	UpdateTearPosion(_DeltaTime);
	CheckForExplosion(_DeltaTime);


}

void ABloodTear::Explosion(float _DeltaTime)
{
	if (nullptr == TearCollision)
	{
		return;
	}

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

void ABloodTear::CheckForExplosion(float _DeltaTime)
{
	if (nullptr == TearCollision)
	{
		return;
	}

	if (false == TearCollision->IsDestroy())
	{
		CollisionActor = TearCollision->CollisionOnce(ECollisionGroup::Player_Body);
	}

	// 플레이어와 충돌하면 터진다.
	if (nullptr != CollisionActor)
	{
		Explosion(_DeltaTime);

		APlayer* CollisionPlayer = dynamic_cast<APlayer*>(CollisionActor);
		CollisionPlayer->ApplyDamaged(ActorAtt);
		CollisionPlayer->ShowHitAnimation(CollisionPlayer);

		UEngineDebug::OutPutString(CollisionPlayer->GetName() + "에게 " + std::to_string(ActorAtt) + " 의 데미지를 주었습니다. // 현재 체력 : " + std::to_string(CollisionPlayer->GetHp()));
	}

	//TimeEventer.PushEvent(Duration, std::bind(&ABloodTear::Explosion, this, std::placeholders::_1));

	TimeElapesd += _DeltaTime;
	if (Duration < TimeElapesd)
	{
		Explosion(_DeltaTime);
	}
}

void ABloodTear::UpdateTearPosion(float _DeltaTime)
{
	FVector2D Result = Dir * Speed;
	AddActorLocation(Dir * Speed * _DeltaTime);
}

void ABloodTear::BeginPlay()
{
	Super::BeginPlay();
}

ABloodTear::~ABloodTear()
{
}