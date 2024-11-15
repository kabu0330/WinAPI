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

// ���� �޾Ƽ� ����� �����Ѵ�.
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
	// �¿� ���ݸ� �߷��� �ش�.
	if (Dir == FVector2D::LEFT || Dir == FVector2D::RIGHT)
	{
		if (GravityActivationTime + 0.1 < TimeElapesd) // �������� �߷����� ����߸���.
		{
			GravityDir = Dir + FVector2D(0, 1);
			AddActorLocation(GravityDir * _DeltaTime * Speed * Gravity);
		}

		else if (GravityActivationTime < TimeElapesd) // �ӵ��� �����.
		{
			GravityDir = Dir + FVector2D(0, 1);
			AddActorLocation(GravityDir * _DeltaTime * Speed * Gravity);
		}
		else if (ResistanceActivationTime < TimeElapesd) // ���׷�
		{
			AddActorLocation(Dir * _DeltaTime * Speed * Resistance);
		}
		else
		{
			AddActorLocation(Dir * _DeltaTime * Speed);
		}
	}
	else // ���Ʒ� ���ݿ��� �߷��� ����.
	{
		if (ResistanceActivationTime < TimeElapesd) // ���׷�
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
	//// 1. ���� �ð��� ������ ������.
	//if (Duration < TimeElapesd)
	//{
	//	Explosion();
	//}
	//// 2. �� ������ ����� ������.
	//ARoom* CurRoom = ARoom::GetCurRoom();
	//U2DCollision* CurRoomCollision = CurRoom->GetRoomCollision();
	//
	////if (true = FTransform::RectToCirCle())
	////{

	////}
	//
	//// 4. ������Ʈ�� �浹�ϸ� ������.

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

	//// 3. ���Ϳ� �浹�ϸ� ������.
	//Explosion();

	//AMonster* CollisionMonster = dynamic_cast<AMonster*>(CollisionActor);
	//CollisionMonster->ApplyDamaged(CollisionActor, ActorAtt);

	//UEngineDebug::OutPutString(CollisionMonster->GetName() + "���� " + std::to_string(ActorAtt) + " �� �������� �־����ϴ�. // ���� ü�� : " + std::to_string(CollisionMonster->GetHp()));

}

void ATear::CollisionSetting()
{
	TearCollision->SetCollisionStay(std::bind(&ATear::Explode, this, std::placeholders::_1));
}

void ATear::Explode(AActor* _Other)
{
	// 1. ���� �ð��� ������ ������.
	TimeBasedExplosion();

	// 2. �� ������ ����� ������.
	BoundaryExplosion();

	/*CollisionActor = TearCollision->CollisionOnce(ECollisionGroup::Object_Wall);
	FTransform CurRoomCollisionTrans = CollisionActor->GetTransform();
	bool Result = FTransform::RectToCirCle(CurRoomCollisionTrans, TearTrans);
	if (true == FTransform::RectToCirCle(CurRoomCollisionTrans, TearTrans))
	{
		Explosion();
	}*/

	// 3. ������Ʈ�� �浹�ϸ� ������.

	// 4. ���Ϳ� �浹�ϸ� ������.
	HandleMonsterCollision(_Other);
}

// �ð� �ʰ� ����
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

// �� ������ �������ϸ� ����
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

// ���Ͷ� �浹�ϸ� ����
void ATear::HandleMonsterCollision(AActor* _Other) // �� ���ʹ� ��κ��� ��� Room���� ���´�.
{
	// �̹� �ı��Ǿ����� ����
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

	// ������Ʈ �浹�̸� ���⼭ ���� �ʴ´�.
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

	// ���� ���� ó��
	AMonster* CollisionMonster = dynamic_cast<AMonster*>(CollisionActor);
	CollisionMonster->ApplyDamaged(CollisionActor, ActorAtt); // �� �Լ��� ü�µ� ��� �ǰ� �ִϸ��̼ǵ� ����Ѵ�.

	UEngineDebug::OutPutString(CollisionMonster->GetName() + "���� " + std::to_string(ActorAtt) + " �� �������� �־����ϴ�. // ���� ü�� : " + std::to_string(CollisionMonster->GetHp()));
}


ATear::~ATear()
{
}