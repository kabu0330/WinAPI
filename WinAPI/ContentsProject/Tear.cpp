#include "PreCompile.h"
#include "Tear.h"

#include <EngineCore/2DCollision.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include <EngineCore/Actor.h>
#include "Player.h"
#include "Monster.h"
#include "PlayGameMode.h"


ATear::ATear()
{
	SetName("Player Tear");
	SetActorLocation(FVector2D::ZERO);

	TearCollision = CreateDefaultSubObject<U2DCollision>();
	TearCollision->SetComponentLocation({ 0, 0 });
	TearCollision->SetComponentScale({ 50, 50 });
	TearCollision->SetCollisionGroup(ECollisionGroup::Player_Attack);
	TearCollision->SetCollisionType(ECollisionType::CirCle);
	
	TearEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	TearEffectRenderer->CreateAnimation("Player_Tear_Normal", "effect_tearpoofa.png", 0, 0, 0, false);
	TearEffectRenderer->CreateAnimation("Player_Tear_Attack", "effect_tearpoofa.png", 1, 15, 0.05f, false);
	TearEffectRenderer->SetComponentScale({ 92, 92 }); // 64, 64
	TearEffectRenderer->SetOrder(ERenderOrder::Tear);
	TearEffectRenderer->ChangeAnimation("Player_Tear_Normal");
	TearEffectRenderer->SetActive(true);

	TearTrans.Location = TearCollision->GetComponentLocation();
	TearTrans.Scale = TearCollision->GetComponentScale();
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

	CollisionSetting();
}

void ATear::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == APlayGameMode::IsGamePaused())
	{
		return;
	}

	if (nullptr == TearCollision)
	{
		return;
	}

	if (false == TearEffectRenderer->IsActive())
	{
		return;
	}

	TimeElapesd += _DeltaTime;
	UpdateTearPosion(_DeltaTime);
	CheckForExplosion(_DeltaTime);
	
}

void ATear::Explosion()
{
	if (nullptr != TearCollision)
	{
		TearCollision->Destroy(); 
		TearCollision = nullptr;
		Dir = FVector2D::ZERO;  
		TearEffectRenderer->ChangeAnimation("Player_Tear_Attack");
		SetActorLocation(GetActorLocation());

		//if (true == TearEffectRenderer->IsCurAnimationEnd())
		//{
		//	Destroy(0.4f);
		//}

		if (nullptr != TearEffectRenderer)
		{
			Destroy(0.4f);
		}
	}
}

void ATear::UpdateTearPosion(float _DeltaTime)
{
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

}

void ATear::CheckForExplosion(float _DeltaTime)
{
	//// 1. 일정 시간이 지나면 터진다.
	//if (Duration < TimeElapesd)
	//{
	//	Explosion();
	//}
	//// 2. 맵 밖으로 벗어나면 터진다.
	//ARoom* CurRoom = ARoom::GetCurRoom();
	//U2DCollision* CurRoomCollision = CurRoom->GetRoomCollision();
	//
	////if (true = FTransform::RectToCirCle())
	////{

	////}
	//
	//// 4. 오브젝트와 충돌하면 터진다.

	//if (nullptr == TearCollision)
	//{
	//	return;
	//}

	//if (false == TearCollision->IsDestroy())
	//{
	//	CollisionActor = TearCollision->CollisionOnce(ECollisionGroup::Monster_Body);
	//}

	//if (nullptr == CollisionActor)
	//{
	//	return;
	//}

	//// 3. 액터와 충돌하면 터진다.
	//Explosion();

	//AMonster* CollisionMonster = dynamic_cast<AMonster*>(CollisionActor);
	//CollisionMonster->ApplyDamaged(CollisionActor, ActorAtt);

	//UEngineDebug::OutPutString(CollisionMonster->GetName() + "에게 " + std::to_string(ActorAtt) + " 의 데미지를 주었습니다. // 현재 체력 : " + std::to_string(CollisionMonster->GetHp()));

}

void ATear::CollisionSetting()
{
	TearCollision->SetCollisionStay(std::bind(&ATear::Explode, this, std::placeholders::_1));
}

void ATear::Explode(AActor* _Other)
{
	// 1. 일정 시간이 지나면 터진다.
	TimeBasedExplosion();

	// 2. 맵 밖으로 벗어나면 터진다.
	BoundaryExplosion();

	/*CollisionActor = TearCollision->CollisionOnce(ECollisionGroup::Object_Wall);
	FTransform CurRoomCollisionTrans = CollisionActor->GetTransform();
	bool Result = FTransform::RectToCirCle(CurRoomCollisionTrans, TearTrans);
	if (true == FTransform::RectToCirCle(CurRoomCollisionTrans, TearTrans))
	{
		Explosion();
	}*/

	// 3. 오브젝트와 충돌하면 터진다.

	// 4. 액터와 충돌하면 터진다.
	HandleMonsterCollision(_Other);
}

// 시간 초과 폭발
void ATear::TimeBasedExplosion() 
{
	if (nullptr == TearCollision)
	{
		return;
	}

	if (Duration < TimeElapesd)
	{
		Explosion();
	}
}

// 맵 밖으로 나가려하면 폭발
void ATear::BoundaryExplosion() 
{
	if (nullptr == TearCollision)
	{
		return;
	}

	ARoom* CurRoom = ARoom::GetCurRoom();
	FVector2D RoomPos = CurRoom->GetActorLocation();
	FVector2D RoomScale = CurRoom->GetActorScale().Half();
	float RoomSizeOffsetX = CurRoom->GetRoomSizeOffsetX() / 2;
	float RoomSizeOffsetY = CurRoom->GetRoomSizeOffsetY() / 2;

	float LeftEdge = RoomPos.X - RoomScale.X - RoomSizeOffsetX;
	float RightEdge = RoomPos.X + RoomScale.X + RoomSizeOffsetX;
	float TopEdge = RoomPos.Y - RoomScale.Y - RoomSizeOffsetY;
	float BotEdge = RoomPos.Y + RoomScale.Y + RoomSizeOffsetY;

	FVector2D Pos = this->GetActorLocation();
	FVector2D TearPos = this->GetActorLocation() + TearCollision->GetComponentLocation();
	if (LeftEdge > TearPos.X)
	{
		Explosion();
	}
	if (RightEdge < TearPos.X)
	{
		Explosion();
	}
	if (TopEdge > TearPos.Y)
	{
		Explosion();
	}
	if (BotEdge < TearPos.Y)
	{
		Explosion();
	}
}

// 몬스터랑 충돌하면 폭발
void ATear::HandleMonsterCollision(AActor* _Other) // 이 액터는 대부분의 경우 Room으로 들어온다.
{
	// 이미 파괴되었으면 리턴
	if (nullptr == TearCollision) 
	{
		return;
	}
	if (true == TearCollision->IsDestroy()) 
	{
		return;
	}

	U2DCollision* Collision = dynamic_cast<U2DCollision*>(_Other);
	if (nullptr == Collision)
	{
		int a = 0;
		return;
	}
	ECollisionGroup CollisionType = static_cast<ECollisionGroup>(Collision->GetGroup());

	// 오브젝트 충돌이면 여기서 하지 않는다.
	if (ECollisionGroup::Object_Wall == CollisionType)
	{
		return;
	}

	CollisionActor = TearCollision->CollisionOnce(CollisionType);
	if (nullptr == CollisionActor)
	{
		return;
	}

	Explosion();

	// 피해 로직 처리
	AMonster* CollisionMonster = dynamic_cast<AMonster*>(CollisionActor);
	CollisionMonster->ApplyDamaged(CollisionActor, ActorAtt); // 이 함수로 체력도 깎고 피격 애니메이션도 출력한다.

	UEngineDebug::OutPutString(CollisionMonster->GetName() + "에게 " + std::to_string(ActorAtt) + " 의 데미지를 주었습니다. // 현재 체력 : " + std::to_string(CollisionMonster->GetHp()));
}


ATear::~ATear()
{
}