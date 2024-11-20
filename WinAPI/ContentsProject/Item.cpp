#include "PreCompile.h"
#include "Item.h"

#include <EngineCore/EngineAPICore.h>
#include "Room.h"
#include "PlayGameMode.h"
#include "Player.h"
#include "Tear.h"

AItem::AItem()
{

	DebugOn();
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	CollisionSetting();
}

void AItem::Tick(float _DeltaTime)
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


	ReverseForce(_DeltaTime);

	ClampPositionToRoom();
	RemoveRoomData();
	ItemDestroy();
}

// Room에 저장된 Item 정보는 삭제한다.
void AItem::RemoveRoomData()
{
	if (false == IsDrop)
	{
		return;
	}

	ParentRoom->RemoveItem(this);
	DropRenderer->SetActive(false);
	PlayerCollision->SetActive(false);
}

void AItem::ItemDestroy()
{
	if (false == IsUseEnd)
	{
		return;
	}
	Destroy();
}

void AItem::CollisionSetting()
{
	if (nullptr != PlayerCollision)
	{
		PlayerCollision->SetCollisionEnter(std::bind(&AItem::Drop, this, std::placeholders::_1));
	}
	if (nullptr != UniversalCollision)
	{
		UniversalCollision->SetCollisionEnter(std::bind(&AItem::AreaWideAttack, this, std::placeholders::_1));
	}

}

void AItem::Drop(AActor* _Player)
{
	APlayer* Player = dynamic_cast<APlayer*>(_Player);
	if (nullptr == Player)
	{
		return;
	}
	
	if (false == Player->Drop(this, ItemCount))
	{
		// 아이템을 먹지 못하고 튕겨내야 함
		FailToPickup(Player);
	}
}

void AItem::DropSuccess()
{
	IsDrop = true;
}

void AItem::FailToPickup(APlayer* _Player)
{
 	FVector2D Dir = GetActorLocation() - _Player->GetActorLocation();
	Dir.Normalize(); // 방향벡터

	Force = Dir * 200.0f;
}

void AItem::ReverseForce(float _DeltaTime)
{
	FVector2D ReverseForce = -Force;
	ReverseForce.Normalize();

	Force += ReverseForce * _DeltaTime * 150.0f;
	
	if (20.0f >= Force.Length())
	{
		Force = FVector2D::ZERO;	
	}

	if (true == IsAtBoundary) // 반사
	{
		FVector2D BoundaryNormal = FVector2D::ZERO;

		if (Force.X > 0) // 오른쪽 경계
		{
			BoundaryNormal = FVector2D(-1.0f, 0.0f);
		}
		else if (Force.X < 0) // 왼쪽 경계
		{
			BoundaryNormal = FVector2D(1.0f, 0.0f);
		}
		else if (Force.Y > 0) // 아래 경계
		{
			BoundaryNormal = FVector2D(0.0f, 1.0f);
		}
		else // 위 경계
		{
			BoundaryNormal = FVector2D(0.0f, -1.0f);
		}

		// 반사 처리
		FVector2D Result =  Reflect(BoundaryNormal);
		Force *= Result.GetNormal();

		IsAtBoundary = false;
	}

	AddActorLocation(Force * _DeltaTime);
}

void AItem::AreaWideAttack(AActor* _Actor)
{
	ARoom* Room = CastActorToType<ARoom>(_Actor);
	if (nullptr != Room)
	{
		return;
	}

	FVector2D Dir = _Actor->GetActorLocation() - GetActorLocation();
	Dir.Normalize(); // 방향벡터
	
	APlayer* Player = CastActorToType<APlayer>(_Actor);
	if (nullptr != Player)
	{	
		Player->ApplyDamaged(Player, 2, Dir); // 폭탄은 플레이어에게 하트 한칸의 피해를 준다.
	}

	AMonster* Monster = CastActorToType<AMonster>(_Actor);
	if (nullptr != Monster)
	{
		Monster->ApplyDamaged(Monster, Att, Dir); // 폭탄은 몬스터에게 60의 피해를 입힌다.
	}

	ARoomObject* Object = CastActorToType<ARoomObject>(_Actor);
	if (nullptr != Object)
	{
		Object->ApplyDamaged(Object);
	}
}

void AItem::Knockback(AActor* _Actor)
{
	if (nullptr == ImpactCollision)
	{
		return;
	}

	ATear* Tear = dynamic_cast<ATear*>(_Actor);
	if (nullptr == Tear)
	{
		return;
	}

	FVector2D Dir = GetActorLocation() - Tear->GetActorLocation();
	Dir.Normalize(); // 방향벡터
	Force = Dir * 100.0f;
}

void AItem::ClampPositionToRoom()
{
	if (nullptr == PlayerCollision)
	{
		return;
	}

	FVector2D Pos = GetActorLocation();
	FVector2D OffsetPos = Pos + PlayerCollision->GetComponentLocation();

	ARoom* CurRoom = ParentRoom;
	FVector2D RoomPos = CurRoom->GetActorLocation();
	FVector2D RoomScale = CurRoom->GetActorScale().Half();
	float RoomSizeOffsetX = CurRoom->GetRoomSizeOffsetX() / 2;
	float RoomSizeOffsetY = CurRoom->GetRoomSizeOffsetY() / 2;

	float LeftEdge = RoomPos.X - RoomScale.X - RoomSizeOffsetX;
	float RightEdge = RoomPos.X + RoomScale.X + RoomSizeOffsetX;
	float TopEdge = RoomPos.Y - RoomScale.Y - RoomSizeOffsetY;
	float BotEdge = RoomPos.Y + RoomScale.Y + RoomSizeOffsetY;

	if (LeftEdge > OffsetPos.X)
	{
		SetActorLocation(Pos + FVector2D{ 1, 0 });
		IsAtBoundary = true;
	}
	if (RightEdge < OffsetPos.X)
	{
		SetActorLocation(Pos + FVector2D{ -1, 0 });
		IsAtBoundary = true;
	}
	if (TopEdge > OffsetPos.Y)
	{
		SetActorLocation(Pos + FVector2D{ 0, 1 });
		IsAtBoundary = true;
	}
	if (BotEdge < OffsetPos.Y)
	{
		SetActorLocation(Pos + FVector2D{ 0, -1 });
		IsAtBoundary = true;
	}
}

FVector2D AItem::Reflect(FVector2D _Dir)
{
	FVector2D ReflectedForce = Force - (_Dir * (2.0f * Force.Dot(_Dir)));
	return ReflectedForce;
}

void AItem::IdleAnimation()
{
	
}

void AItem::SpawnAnimation()
{
}

void AItem::HoverAnimation()
{
}

void AItem::GlowAnimation()
{
}

AItem::~AItem()
{
}
