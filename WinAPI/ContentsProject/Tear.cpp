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

// 값만 받아서 멤버에 저장한다.
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
	// 1. 일정 시간이 지나면 터진다.
	TimeBasedExplosion();

	// 2. 맵 밖으로 벗어나면 터진다.
	BoundaryExplosion(_Other);

	// 3. 오브젝트와 충돌하면 터진다.
	MapObjectCollision(_Other);

	// 4. 액터와 충돌하면 터진다.
	HandleMonsterCollision(_Other);

	// 5. 특정 아이템과 충돌하면 터진다.
	ItemImpackCollision(_Other);
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
void ATear::BoundaryExplosion(AActor* _Other)
{
	ARoom* CurRoom = ARoom::GetCurRoom();
	FVector2D RoomPos = CurRoom->GetActorLocation();
	FVector2D RoomScale = CurRoom->GetActorScale().Half();
	float RoomSizeOffsetX = CurRoom->GetRoomSizeOffsetX() / 2;
	float RoomSizeOffsetY = CurRoom->GetRoomSizeOffsetY() / 2;

	float Offset = 60.0f; // 맵 경계면보다 더 멀리 나가서 터지도록 설정하고 싶다.
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

	UEngineDebug::OutPutString(CollisionOther->GetName() + "에게 " + std::to_string(1) + " 의 데미지를 주었습니다. // 현재 체력 : " + std::to_string(CollisionOther->GetHp()));
}

// 특정 아이템과 충돌하면 밀어내기
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
	// 특수 충돌그룹 로직 설정

	CollisionOther->Knockback(this);

	Explosion();

	UEngineDebug::OutPutString(CollisionOther->GetName() + "에게 넉백 공격을 가했습니다.");
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

	AMonster* CollisionMonster = dynamic_cast<AMonster*>(_Other);
	if (nullptr == CollisionMonster)
	{
		return;
	}
	if (nullptr == CollisionMonster->GetBodyCollision())
	{
		return;
	}

	// Host 전용 로직
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
	
	// 충돌체의 정보는 모두 Actor로 넘겨줘야 한다.
	ECollisionGroup CollisionType = static_cast<ECollisionGroup>(CollisionMonster->GetBodyCollision()->GetGroup());
	CollisionActor = TearCollision->CollisionOnce(CollisionType);
	if (nullptr == CollisionActor)
	{
		return;
	}

	Explosion();

	// 피해 로직 처리
	// 충돌체의 정보는 모두 Actor로 넘겨줘야 한다.
	CollisionMonster->ApplyDamaged(CollisionActor, ActorAtt, KnockbackDir); // 이 함수로 체력도 깎고 피격 애니메이션도 출력한다.

	UEngineDebug::OutPutString(CollisionMonster->GetName() + "에게 " + std::to_string(ActorAtt) + " 의 데미지를 주었습니다. // 현재 체력 : " + std::to_string(CollisionMonster->GetHp()));
}


ATear::~ATear()
{
}