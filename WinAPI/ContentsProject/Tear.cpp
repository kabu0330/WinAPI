#include "PreCompile.h"
#include "Tear.h"

#include <EngineCore/2DCollision.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include <EngineCore/Actor.h>
#include "Player.h"
#include "Monster.h"


ATear::ATear()
{
	SetActorLocation(FVector2D::ZERO);

	TearCollision = CreateDefaultSubObject<U2DCollision>();
	TearCollision->SetComponentLocation({ 0, 0 });
	TearCollision->SetComponentScale({ 25, 25 });
	TearCollision->SetCollisionGroup(ECollisionGroup::PLAYER_ATTACK);
	TearCollision->SetCollisionType(ECollisionType::CirCle);
	
	TearEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	TearEffectRenderer->CreateAnimation("Player_Tear_Normal", "effect_tearpoofa.png", 0, 0, 0, false);
	TearEffectRenderer->CreateAnimation("Player_Tear_Attack", "effect_tearpoofa.png", 1, 15, 0.05f, false);
	TearEffectRenderer->SetComponentScale({ 96, 96 }); // 64, 64
	TearEffectRenderer->SetOrder(ERenderOrder::TEAR);
	TearEffectRenderer->ChangeAnimation("Player_Tear_Normal");
	TearEffectRenderer->SetActive(true);

	DebugOn();
}

// 값만 받아서 멤버에 저장한다.
void ATear::Fire(FVector2D _StartPos, FVector2D _Dir, float _Speed, int _Att)
{
	TearEffectRenderer->SetActive(true);
	SetActorLocation(_StartPos);
	Dir = _Dir;
	ActorAtt = _Att;

	APlayer* Player = dynamic_cast<APlayer*>(GetWorld()->GetPawn());
	float FinalSpeed = 0.0f;
	if (true == Player->PlayerIsMove())
	{
		FinalSpeed = SpeedMax;	
	}
	else
	{
		FinalSpeed = Speed;
	}

	Speed = FinalSpeed / Duration;
	if (Speed >= SpeedMax)
	{
		Speed = SpeedMax;
		ResistanceActivationTime = 0.2f;
		Resistance = 0.9f;
		GravityActivationTime = 0.5f;
	}
}

void ATear::BeginPlay()
{
	Super::BeginPlay();
}

void ATear::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (false == TearEffectRenderer->IsActive())
	{
		return;
	}
	
	TimeElapesd += _DeltaTime;

	// 좌우 공격만 중력을 준다.
	if (Dir == FVector2D::LEFT || Dir == FVector2D::RIGHT)
	{
		if (GravityActivationTime + 0.1 < TimeElapesd) // 마지막에 중력으로 떨어뜨린다.
		{
			GravityDir = Dir + FVector2D(0, 1);
			AddActorLocation(GravityDir * _DeltaTime * Speed * Gravity);
		}

		else if (GravityActivationTime < TimeElapesd) // 속도를 늦춘다.
		{
			GravityDir = Dir + FVector2D(0, 1);
			AddActorLocation(GravityDir * _DeltaTime * Speed * Gravity);
		}
		else if (ResistanceActivationTime < TimeElapesd) // 저항력
		{
			AddActorLocation(Dir * _DeltaTime * Speed * Resistance);
		}
		else
		{
			AddActorLocation(Dir * _DeltaTime * Speed);
		}
	}
	else // 위아래 공격에는 중력이 없다.
	{
		if (ResistanceActivationTime < TimeElapesd) // 저항력
		{
			AddActorLocation(Dir * _DeltaTime * Speed * Resistance);
		}
		else
		{
			AddActorLocation(Dir * _DeltaTime * Speed);
		}
	}
	
	if (nullptr != TearCollision)
	{
		if (false == TearCollision->IsDestroy()) 
		{
			CollisionActor = TearCollision->CollisionOnce(ECollisionGroup::MONSTER_BODY);
		}

		// 3. 액터와 충돌하면 터진다.
		if (nullptr != CollisionActor)
		{
			TriggerExplosion(_DeltaTime);


			AMonster* CollisionMonster = dynamic_cast<AMonster*>(CollisionActor);
			CollisionMonster->ApplyDamaged(ActorAtt);
			
			UEngineDebug::OutPutString(CollisionMonster->GetName() + "에게 " + std::to_string(ActorAtt) + " 의 데미지를 주었습니다. // 현재 체력 : " + std::to_string(CollisionMonster->GetHp()));
		}
	}


	// 1. 일정 시간이 지나면 터진다.
	if (Duration < TimeElapesd)
	{
		TriggerExplosion(_DeltaTime);	
	}

	// 2. 맵 밖으로 벗어나면 터진다.
	// 4. 오브젝트와 충돌하면 터진다.
}

void ATear::TriggerExplosion(float _DeltaTime)
{
	if (nullptr != TearCollision)
	{
		TearCollision->Destroy(); 
		TearCollision = nullptr;
		Dir = FVector2D::ZERO;  
		TearEffectRenderer->ChangeAnimation("Player_Tear_Attack");
		SetActorLocation(GetActorLocation());

		if (nullptr != TearEffectRenderer)
		{
			Destroy(0.4f);
		}
	}
}

ATear::~ATear()
{
}