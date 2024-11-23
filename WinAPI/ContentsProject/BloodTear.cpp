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
	TearCollision->SetComponentScale({ 23, 23 });
	TearCollision->SetCollisionGroup(ECollisionGroup::Monster_Attack);
	TearCollision->SetCollisionType(ECollisionType::Circle);

	TearEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	TearEffectRenderer->CreateAnimation("BloodTear_Normal", "effect_bloodtear.png", 0, 0, 0, false);
	TearEffectRenderer->CreateAnimation("BloodTear_Attack", "effect_bloodtear.png", 1, 15, 0.05f, false);
	TearEffectRenderer->SetComponentScale({ 96, 96 }); // 64, 64
	TearEffectRenderer->SetOrder(ERenderOrder::Tear);
	TearEffectRenderer->ChangeAnimation("BloodTear_Normal");
	TearEffectRenderer->SetActive(true);


	DebugOn();
}

void ABloodTear::Fire(FVector2D _StartPos, FVector2D _Dir, float _Speed,  int _Att)
{
	TearEffectRenderer->SetActive(true);
	SetActorLocation(_StartPos);
	Dir = _Dir; // Dir�� ���� �� �����ͷ� ������ �ϹǷ�
	KnockbackDir = _Dir; // ���ƿ��� ������ ��� ������ ���Ͱ� �ʿ��ϴ�.
	Speed = _Speed;
	ActorAtt = _Att;
}

void ABloodTear::Tick(float _DeltaTime)
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

void ABloodTear::Explosion()
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

}

void ABloodTear::CollisionSetting()
{
	TearCollision->SetCollisionStay(std::bind(&ABloodTear::Explode, this, std::placeholders::_1));
}

void ABloodTear::Explode(AActor* _Other)
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

void ABloodTear::TimeBasedExplosion()
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

void ABloodTear::BoundaryExplosion()
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

void ABloodTear::MapObjectCollision(AActor* _Other)
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

	Explosion();
}

void ABloodTear::HandleMonsterCollision(AActor* _Other)
{
	if (nullptr == TearCollision)
	{
		return;
	}

	if (true == TearCollision->IsDestroy())
	{
		return;
	}

	CollisionActor = TearCollision->CollisionOnce(ECollisionGroup::Player_Body);
	if (nullptr == CollisionActor)
	{
		return;
	}

	Explosion();

	APlayer* CollisionPlayer = dynamic_cast<APlayer*>(CollisionActor);
	CollisionPlayer->ApplyDamaged(CollisionActor, ActorAtt, KnockbackDir);
	//CollisionPlayer->ShowHitAnimation(CollisionPlayer);

	UEngineDebug::OutPutString(CollisionPlayer->GetName() + "���� " + std::to_string(ActorAtt) + " �ǵ������� �־����ϴ�. // ���� ü�� : " + std::to_string(CollisionPlayer->GetHp()));


}

void ABloodTear::UpdateTearPosion(float _DeltaTime)
{
	FVector2D Result = Dir * Speed;
	AddActorLocation(Dir * Speed * _DeltaTime);
}

void ABloodTear::BeginPlay()
{
	Super::BeginPlay();

	CollisionSetting();
}

ABloodTear::~ABloodTear()
{
}