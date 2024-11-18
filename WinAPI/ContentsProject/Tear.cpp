#include "PreCompile.h"
#include "Tear.h"
#include <cmath>

#include <EngineCore/2DCollision.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsEnum.h"
#include <EngineCore/Actor.h>
#include "Player.h"
#include "Monster.h"
#include "PlayGameMode.h"
#include "RoomObject.h"


ATear::ATear()
{
	SetName("Player Tear");
	SetActorLocation(FVector2D::ZERO);

	TearCollision = CreateDefaultSubObject<U2DCollision>();
	TearCollision->SetComponentLocation({ 0, 0 });
	TearCollision->SetComponentScale({ 50, 50 });
	TearCollision->SetCollisionGroup(ECollisionGroup::Player_Attack);
	TearCollision->SetCollisionType(ECollisionType::Circle);
	
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
	KnockbackDir = _Dir;
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

	// 3. ������Ʈ�� �浹�ϸ� ������.
	MapObjectCollision(_Other);

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

	float Offset = 37.0f; // �� ���麸�� �� �ָ� ������ �������� �����ϰ� �ʹ�.
	int LeftEdge  = static_cast<int>(std::round(RoomPos.X - RoomScale.X - RoomSizeOffsetX - Offset));
	int RightEdge = static_cast<int>(std::round(RoomPos.X + RoomScale.X + RoomSizeOffsetX + Offset));
	int TopEdge   = static_cast<int>(std::round(RoomPos.Y - RoomScale.Y - RoomSizeOffsetY - Offset));
	int BotEdge   = static_cast<int>(std::round(RoomPos.Y + RoomScale.Y + RoomSizeOffsetY + Offset));

	FVector2D TearCenter = this->GetActorLocation();
	FVector2D TearSize = TearCollision->GetComponentScale().Half();

	int TearLeft   = static_cast<int>(std::round(TearCenter.X - TearSize.X));
	int TearRight  = static_cast<int>(std::round(TearCenter.X + TearSize.X));
	int TearTop    = static_cast<int>(std::round(TearCenter.Y - TearSize.Y));
	int TearBottom = static_cast<int>(std::round(TearCenter.Y + TearSize.Y));

	if (LeftEdge >= TearLeft)
	{
		Explosion();
	}
	if (RightEdge <= TearRight)
	{
		Explosion();
	}
	if (TopEdge >= TearTop)
	{
		Explosion();
	}
	if (BotEdge <= TearBottom)
	{
		Explosion();
	}
}

void ATear::MapObjectCollision(AActor* _Other)
{
	ARoomObject* CollisionOther = dynamic_cast<ARoomObject*>(_Other);
	if (nullptr == CollisionOther)
	{
		return;
	}
	ECollisionGroup CollisionType = static_cast<ECollisionGroup>(CollisionOther->GetBodyCollision()->GetGroup());

	CollisionActor = TearCollision->CollisionOnce(CollisionType);
	if (nullptr == CollisionActor)
	{
		return;
	}
	// Ư�� �浹�׷� ���� ����


	Explosion();
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

	AMonster* CollisionMonster = dynamic_cast<AMonster*>(_Other);
	if (nullptr == CollisionMonster)
	{
		return;
	}
	if (nullptr == CollisionMonster->GetBodyCollision())
	{
		return;
	}

	// Host ���� ����
	if (true == CollisionMonster->GetHeadCollision()->IsActive())
	{
		ECollisionGroup CollisionHeadType = static_cast<ECollisionGroup>(CollisionMonster->GetHeadCollision()->GetGroup());

		if (ECollisionGroup::Monster_Barrier == CollisionHeadType)
		{
			Explosion(); 
			return;
		}
	}

	// �浹ü�� ������ ��� Actor�� �Ѱ���� �Ѵ�.
	ECollisionGroup CollisionType = static_cast<ECollisionGroup>(CollisionMonster->GetBodyCollision()->GetGroup());
	CollisionActor = TearCollision->CollisionOnce(CollisionType);
	if (nullptr == CollisionActor)
	{
		return;
	}

	Explosion();

	// ���� ���� ó��
	// �浹ü�� ������ ��� Actor�� �Ѱ���� �Ѵ�.
	CollisionMonster->ApplyDamaged(CollisionActor, ActorAtt, KnockbackDir); // �� �Լ��� ü�µ� ��� �ǰ� �ִϸ��̼ǵ� ����Ѵ�.

	UEngineDebug::OutPutString(CollisionMonster->GetName() + "���� " + std::to_string(ActorAtt) + " �� �������� �־����ϴ�. // ���� ü�� : " + std::to_string(CollisionMonster->GetHp()));
}


ATear::~ATear()
{
}