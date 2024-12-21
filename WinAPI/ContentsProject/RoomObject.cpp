#include "PreCompile.h"
#include "RoomObject.h"
#include <EngineCore/EngineAPICore.h>
#include "Room.h"
#include "PlayGameMode.h"
#include "Bomb.h"
#include "Rock.h"
#include "Heart.h"
#include "Penny.h"
#include "Bomb.h"
#include "Key.h"
#include "DecalObject.h"

ARoomObject::ARoomObject()
{
	DebugOn();
}

void ARoomObject::BeginPlay()
{
	Super::BeginPlay();
	CollisionSetting();
}

void ARoomObject::Tick(float _DeltaTime)
{

	// 플레이어와 다른 맵이면 리턴
	ARoom* PlayerCurRoom = ARoom::GetCurRoom();
	if (PlayerCurRoom != ParentRoom) 
	{
		return;
	}
	// 게임이 일시정지라면 모두 정지
	if (true == APlayGameMode::IsGamePaused()) 
	{
		return;
	}

	Super::Tick(_DeltaTime);

	SwitchAnimation(); // 눈물, 폭탄 등과 상호작용해서 이미지가 바뀌어야 하는 경우
	ChangeRenderOrder(); // 렌더 이미지 크기와 충돌 범위가 차이가 날 경우, 차이나는 상단의 렌더 순서를 변경하여 플레이가 뒤에 그려지도록 변경
}

void ARoomObject::ChangeRenderOrder()
{
	if (true == IsDeath())
	{
		return;
	}

	if (nullptr == BodyRenderer)
	{
		return;
	}
	if (nullptr == BlockingPathCollision)
	{
		return;
	}

	APlayer* Player = dynamic_cast<APlayer*>(GetWorld()->GetPawn());
	if (nullptr == Player)
	{
		return;
	}
	FVector2D PlayerPos = Player->GetActorLocation();

	FVector2D ObjectPos = GetActorLocation();
	FVector2D RenderPivot = BodyRenderer->GetComponentScale().Half();
	FVector2D CollisionPivot = BlockingPathCollision->GetComponentScale().Half() - BlockingPathCollision->GetComponentLocation().Half();
	FVector2D Pivot = RenderPivot - CollisionPivot;
	FVector2D ObjectPivotPos = ObjectPos - Pivot;
	
	if (PlayerPos.Y < ObjectPivotPos.Y)
	{
		BodyRenderer->SetOrder(ERenderOrder::Object_Front);
		return;
	}
	else
	{
		BodyRenderer->SetOrder(ERenderOrder::Object_Back);
		return;
	}
}

void ARoomObject::CollisionSetting()
{
	if (nullptr == BodyCollision)
	{
		return;
	}
	BodyCollision->SetCollisionStay(std::bind(&ARoomObject::DealDamageToPlayer, this, std::placeholders::_1));
	//BodyCollision->SetCollisionStay(std::bind(&ARoomObject::ApplyBomb, this, std::placeholders::_1));

	if (nullptr == BlockingPathCollision)
	{
		return;
	}
	BlockingPathCollision->SetCollisionStay(std::bind(&ARoomObject::Blocker, this, std::placeholders::_1));
}

void ARoomObject::Blocker(AActor* _Actor)
{
	// 플레이어나 몬스터가 지나갈 수 없도록하는 로직
	if (_Actor == nullptr)
	{
		return;
	}
	if (false == IsBlockingPath) // 길막 오브젝트는 모두 true해야함
	{
		return;
	}

	// Actor의 이동을 차단하는 로직
	FVector2D ActorPos = _Actor->GetActorLocation();
	APlayer * Player = dynamic_cast<APlayer*>(_Actor);
	if (nullptr != Player)
	{
		BlockPlayerCollision(Player, ActorPos);
		return;
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Actor); // 몬스터라면
	if (true)
	{
		BlockMonsterCollision(Monster, ActorPos);
		return;
	}
}

int ARoomObject::ApplyDamaged(AActor* _Actor)
{
	if (true == IsDeath())
	{
		return Hp;
	}


	if (true == IsTearDamageable)
	{
		Hp -= 1;
		IsCreateGib = true;

		if (Hp <= 0)
		{
			Hp = 0;
			DestroyCollision();
		}
		return Hp;
	}

	ABomb* Bomb = dynamic_cast<ABomb*>(_Actor);
	if (nullptr == Bomb)
	{
		return Hp;
	}
	if (true == CanExplode) // 파괴가 가능한 오브젝트라면
	{
		// 디스트로이 여부를 자식 클래스에서 결정해줘야 할듯 Hp = 0;
		DestroyRenderer();
		return Hp;
	}

	return Hp;
}

void ARoomObject::SwitchAnimation()
{
	this;
	if (false == IsTearDamageable)
	{
		return;
	}
	if ("" == ObjectName)
	{
		return;
	}

	switch (Hp)
	{
	case 3:
		BodyRenderer->ChangeAnimation(ObjectName);
		break;
	case 2:
		BodyRenderer->ChangeAnimation(ObjectName + std::to_string(1));

		if (true == IsCreateGib)
		{
			CreateGib();
			IsCreateGib = false;
		}
		break;
	case 1:
		BodyRenderer->ChangeAnimation(ObjectName + std::to_string(2));

		if (true == IsCreateGib)
		{
			CreateGib();
			IsCreateGib = false;
		}
		break;
	case 0: // 충돌체가 파괴되고 남은 잔해물
		BodyRenderer->ChangeAnimation(ObjectName + std::to_string(3));
		BodyRenderer->SetOrder(ERenderOrder::Object_PlayerBack);

		if (true == IsCreateGib)
		{
			CreateGib();
			IsCreateGib = false;
		}
		break;
	default:
		break;
	}
}

void ARoomObject::DestroyCollision()
{
	if (nullptr == BodyCollision)
	{
		return;
	}

	IsBlockingPath = false;

	BodyCollision->Destroy();
	BodyCollision = nullptr;
	SpawnItem();

	if (nullptr != BlockingPathCollision)
	{
		BlockingPathCollision->Destroy();
		BlockingPathCollision = nullptr;
	}
}

bool ARoomObject::IsDeath()
{
	this;
	if (0 >= Hp)
	{
		IsDead = true;

		CanExplode = false;
		//IsTearDamageable = false; 
		IsBlockingPath = false;
		IsAttackable = false;
		return true;
	}
	return false;
}

void ARoomObject::DealDamageToPlayer(AActor* _Actor)
{
	if (false == IsAttackable)
	{
		return;
	}

	APlayer* Player = dynamic_cast<APlayer*>(_Actor);
	if (nullptr == Player)
	{
		return;
	}

	Player->ApplyDamaged(Player, 1, FVector2D::ZERO);

}

void ARoomObject::SpawnItem()
{
	UEngineRandom Random;
	Random.SetSeed(time(NULL));
	int Result = Random.RandomInt(0, 7);
	FVector2D Offset = FVector2D(0, -5);
	if (4 > Result)
	{
		ARoomObject* ApperanceEffect = ParentRoom->CreateObject<ADecalObject>(this, Offset);
		USpriteRenderer* Renderer = ApperanceEffect->GetBodyRenderer();
		Renderer->ChangeAnimation("ApperanceEffect");
		Renderer->SetComponentScale({ 256, 256 });
		Renderer->SetOrder(ERenderOrder::ItemEffect);
		ApperanceEffect->Destroy(0.2f);
	}

	switch (Result)
	{
	case 0:
	{
		ParentRoom->CreateItem<AHeart>(this, Offset);
		break;
	}
	case 1:
	{
		ParentRoom->CreateItem<APenny>(this, Offset);
		break;
	}
	case 2:
	{
		ParentRoom->CreateItem<ABomb>(this, Offset);
		break;
	}
	case 3:
	{
		ParentRoom->CreateItem<AKey>(this, Offset);
		break;
	}
	case 4:
	{
		AItem* HalfHeart = ParentRoom->CreateItem<AHeart>(this, Offset);
		HalfHeart->SetHalfHeart();
		break;
	}
	default:
		break;
	}
}

void ARoomObject::BlockPlayerCollision(APlayer* _Player, FVector2D _Pos)
{
	if (false == IsBlockingPath)
	{
		return;
	}
	if (nullptr == _Player)
	{
		return;
	}
	if (nullptr == _Player->GetWarpCollision())
	{
		return;
	}

	FVector2D ActorPos = _Pos;
	FVector2D FootPos = ActorPos + _Player->GetWarpCollision()->GetComponentLocation();
	FVector2D Velocity = _Player->GetFinalSpeed();

	float ActorLeftPos = _Pos.X - _Player->GetWarpCollision()->GetComponentScale().Half().X;
	float ActorRightPos = _Pos.X + _Player->GetWarpCollision()->GetComponentScale().Half().X;
	float ActorTopPos = _Pos.Y - _Player->GetWarpCollision()->GetComponentScale().Half().Y;
	float ActorBotPos = _Pos.Y + _Player->GetWarpCollision()->GetComponentScale().Half().Y;

	FVector2D ThisPos = GetActorLocation();
	FVector2D ThisScale = BodyCollision->GetComponentScale().Half();

	float LeftEdge = ThisPos.X - ThisScale.X;
	float RightEdge = ThisPos.X + ThisScale.X;
	float TopEdge = ThisPos.Y - ThisScale.Y;
	float BotEdge = ThisPos.Y + ThisScale.Y;

	bool IsCollidedX = (ActorLeftPos < RightEdge && ActorRightPos > LeftEdge);
	bool IsCollidedY = (ActorTopPos < BotEdge && ActorBotPos > TopEdge);

	if (IsCollidedX && IsCollidedY)
	{
		// 충돌 방향 계산
		FVector2D CollisionNormal(0.0f, 0.0f);

		if (ActorLeftPos < LeftEdge)  // 왼쪽 충돌
		{
			CollisionNormal.X = -1.0f;
		}
		else if (ActorRightPos > RightEdge)  // 오른쪽 충돌
		{
			CollisionNormal.X = 1.0f;
		}

		if (ActorTopPos < TopEdge)  // 위쪽 충돌
		{
			CollisionNormal.Y = -1.0f;
		}
		else if (ActorBotPos > BotEdge)  // 아래쪽 충돌
		{
			CollisionNormal.Y = 1.0f;
		}

		// 이동 속도 감소
		float NewSpeed = UEngineMath::Min(Velocity.Length() * 0.9f, 250.0f); 
		FVector2D NewVelocity = Velocity.GetNormal() * NewSpeed;

		// 충돌 반대 방향으로 밀어내기
		FVector2D Offset = CollisionNormal * 2.0f; // 밀어내는 정도
		FVector2D NewPosition = ActorPos + Offset;

		// 플레이어 위치와 속도 설정
		_Player->SetActorLocation(NewPosition);
		_Player->SetFinalSpeed(NewVelocity);
	}
}

void ARoomObject::BlockMonsterCollision(AMonster* _Monster, FVector2D _Pos)
{
	if (false == IsBlockingPath)
	{
		return;
	}
	if (nullptr == _Monster)
	{
		return;
	}
	if (nullptr == _Monster->GetBodyCollision())
	{
		return;
	}

	FVector2D ActorPos = _Pos;
	float ActorLeftPos = _Pos.X - _Monster->GetBodyCollision()->GetComponentScale().Half().X;
	float ActorRightPos = _Pos.X + _Monster->GetBodyCollision()->GetComponentScale().Half().X;
	float ActorTopPos = _Pos.Y - _Monster->GetBodyCollision()->GetComponentScale().Half().Y;
	float ActorBotPos = _Pos.Y + _Monster->GetBodyCollision()->GetComponentScale().Half().Y;

	FVector2D Velocity = _Monster->GetDirection();

	FVector2D ThisPos = GetActorLocation();
	FVector2D ThisScale = BodyCollision->GetComponentScale().Half();

	float LeftEdge = ThisPos.X - ThisScale.X;
	float RightEdge = ThisPos.X + ThisScale.X;
	float TopEdge = ThisPos.Y - ThisScale.Y;
	float BotEdge = ThisPos.Y + ThisScale.Y;

	bool IsCollidedX = (ActorLeftPos < RightEdge && ActorRightPos > LeftEdge);
	bool IsCollidedY = (ActorTopPos < BotEdge && ActorBotPos > TopEdge);

	if (IsCollidedX && IsCollidedY)
	{
		// 충돌 방향 계산
		FVector2D CollisionNormal(0.0f, 0.0f);

		if (ActorLeftPos < LeftEdge)  // 왼쪽 충돌
		{
			CollisionNormal.X = -1.0f;
		}
		else if (ActorRightPos > RightEdge)  // 오른쪽 충돌
		{
			CollisionNormal.X = 1.0f;
		}

		if (ActorTopPos < TopEdge)  // 위쪽 충돌
		{
			CollisionNormal.Y = -1.0f;
		}
		else if (ActorBotPos > BotEdge)  // 아래쪽 충돌
		{
			CollisionNormal.Y = 1.0f;
		}

		// 충돌 반대 방향으로 밀어내기
		FVector2D Offset = CollisionNormal * 1.0f; // 밀어내는 정도
		FVector2D NewPosition = ActorPos + Offset;

		// 몬스터 위치와 속도 설정
		_Monster->SetActorLocation(NewPosition);
		_Monster->SetDirection(Velocity);
	}
}

ARoomObject::~ARoomObject()
{
}