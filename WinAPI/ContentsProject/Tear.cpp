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
#include "Item.h"

ATear::ATear()
{
	SetName("Player Tear");
	SetActorLocation(FVector2D::ZERO);

	RendererScale = { 92, 92 };
	CollisionScale = { 24, 24 };

	TearCollision = CreateDefaultSubObject<U2DCollision>();
	TearCollision->SetComponentLocation({ 2, -1 });
	TearCollision->SetComponentScale(CollisionScale);
	TearCollision->SetCollisionGroup(ECollisionGroup::Player_Attack);
	TearCollision->SetCollisionType(ECollisionType::Circle);
	
	TearEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	TearEffectRenderer->SetComponentScale(RendererScale); // 64, 64
	TearEffectRenderer->SetOrder(ERenderOrder::Tear);

	DebugOn();
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

	ApplyGravity(Gravity * _DeltaTime);
	ApplyForce(_DeltaTime);

	UpdateTearPosion(_DeltaTime);
}

// ���� �޾Ƽ� ����� �����Ѵ�.
void ATear::Fire(APlayer* _Player, AItem* _Item, FVector2D _StartPos, FVector2D _Dir, int _Att, float _TearSpeed,  float _ItemTearSpeed, float _Duration, FVector2D _Scale)
{
	Player = _Player;

	if(nullptr == _Item)
	{
		TearEffectRenderer->CreateAnimation("Player_Tear_Normal", "effect_tearpoofa.png", 0, 0, 0, false);
		TearEffectRenderer->CreateAnimation("Player_Tear_Attack", "effect_tearpoofa.png", 1, 15, 0.05f, false);
	}

	TearEffectRenderer->ChangeAnimation("Player_Tear_Normal");
	TearEffectRenderer->SetActive(true);

 	TearEffectRenderer->TestDebugOn();

	SetActorLocation(_StartPos);


	if (FVector2D::UP == _Dir || FVector2D::DOWN == _Dir)
	{
		IsUpDownDir = true;
		Duration -= 0.1f;
	}
	Dir = _Dir.GetNormal();
	KnockbackDir = _Dir;

	ActorAtt = _Att;

	float DefulatSpeed = 600.0f;
	float PlayerMinSpeed = 200.0f;
	float PlayerSpeed = (_TearSpeed <= PlayerMinSpeed) ? DefulatSpeed : DefulatSpeed + ((_TearSpeed - PlayerMinSpeed) * 0.3f);
	TearSpeed = PlayerSpeed + _ItemTearSpeed;

	Velocity = Dir * TearSpeed;

	Duration += _Duration;

	
	CollisionScale += _Scale * 0.5f;
	RendererScale += _Scale * 2.67f;

	TearCollision->SetComponentScale(CollisionScale);
	TearEffectRenderer->SetComponentScale(RendererScale); 
}

void ATear::UpdateTearPosion(float _DeltaTime)
{
	Velocity += Force;
	FVector2D Result = Velocity;

	int a = 0;

	if (Velocity.Length() > TearMaxSpeed)
	{
		Velocity = Velocity.GetNormal() * TearMaxSpeed;
	}

	AddActorLocation(Velocity * _DeltaTime);
	
	Force = FVector2D::ZERO;
}

void ATear::ApplyGravity(FVector2D _Gravity)
{
	if (true == IsUpDownDir)
	{
		return;
	}
	if (TimeElapesd < GravityActivationTime)
	{
		return;
	}

	TearCollision->SetComponentScale({ 0, 0 });
	Force += _Gravity;
}

void ATear::ApplyForce(float _DeltaTime)
{
	FVector2D Reverse = -Velocity;
	Reverse.Normalize();
	Reverse = Reverse * 400.0f * _DeltaTime;
	Force += Reverse;
}

void ATear::Explosion()
{
	if (nullptr != TearCollision)
	{
		Sound = UEngineSound::Play("tear_block.wav");

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

void ATear::CollisionSetting()
{
	TearCollision->SetCollisionStay(std::bind(&ATear::Explode, this, std::placeholders::_1));
}

void ATear::Explode(AActor* _Other)
{
	// 1. ���� �ð��� ������ ������.
	TimeBasedExplosion();

	// 2. �� ������ ����� ������.
	BoundaryExplosion(_Other);

	// 3. ������Ʈ�� �浹�ϸ� ������.
	MapObjectCollision(_Other);

	// 4. ���Ϳ� �浹�ϸ� ������.
	HandleMonsterCollision(_Other);

	// 5. Ư�� �����۰� �浹�ϸ� ������.
	ItemImpackCollision(_Other);
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
void ATear::BoundaryExplosion(AActor* _Other)
{
	ARoom* CurRoom = ARoom::GetCurRoom();
	FVector2D RoomPos = CurRoom->GetActorLocation();
	FVector2D RoomScale = CurRoom->GetActorScale().Half();
	float RoomSizeOffsetX = CurRoom->GetRoomSizeOffsetX() / 2;
	float RoomSizeOffsetY = CurRoom->GetRoomSizeOffsetY() / 2;

	float Offset = 60.0f; // �� ���麸�� �� �ָ� ������ �������� �����ϰ� �ʹ�.
	int LeftEdge  = static_cast<int>(std::round(RoomPos.X - RoomScale.X - RoomSizeOffsetX - Offset));
	int RightEdge = static_cast<int>(std::round(RoomPos.X + RoomScale.X + RoomSizeOffsetX + Offset));
	int TopEdge   = static_cast<int>(std::round(RoomPos.Y - RoomScale.Y - RoomSizeOffsetY - Offset));
	int BotEdge   = static_cast<int>(std::round(RoomPos.Y + RoomScale.Y + RoomSizeOffsetY + Offset));

	FVector2D TearCenter = this->GetActorLocation();
	FVector2D TearSize = { 32, 32 };

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
	
	if (nullptr == TearCollision)
	{
		return;
	}
	ECollisionGroup CollisionType = static_cast<ECollisionGroup>(CollisionOther->GetBodyCollision()->GetGroup());


	CollisionActor = TearCollision->CollisionOnce(CollisionType);
	if (nullptr == CollisionActor)
	{
		return;
	}

	CollisionOther->ApplyDamaged(CollisionActor);

	Explosion();

	UEngineDebug::OutPutString(CollisionOther->GetName() + "���� " + std::to_string(1) + " �� �������� �־����ϴ�. // ���� ü�� : " + std::to_string(CollisionOther->GetHp()));
}

// Ư�� �����۰� �浹�ϸ� �о��
void ATear::ItemImpackCollision(AActor* _Other)
{
	AItem* CollisionOther = dynamic_cast<AItem*>(_Other);
	if (nullptr == CollisionOther)
	{
		return;
	}

	CollisionActor = TearCollision->CollisionOnce(ECollisionGroup::Item_Impact);
	if (nullptr == CollisionActor)
	{
		return;
	}
	// Ư�� �浹�׷� ���� ����

	CollisionOther->Knockback(this);

	Explosion();

	UEngineDebug::OutPutString(CollisionOther->GetName() + "���� �˹� ������ ���߽��ϴ�.");
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
	if (nullptr != CollisionMonster->GetHeadCollision())
	{
		if (true == CollisionMonster->GetHeadCollision()->IsActive())
		{
			ECollisionGroup CollisionHeadType = static_cast<ECollisionGroup>(CollisionMonster->GetHeadCollision()->GetGroup());

			if (ECollisionGroup::Monster_Barrier == CollisionHeadType)
			{
				Explosion();
				return;
			}
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