#include "PreCompile.h"
#include "Item.h"

#include <EngineCore/EngineAPICore.h>
#include "Room.h"
#include "PlayGameMode.h"
#include "Player.h"
#include "Tear.h"

AItem::AItem()
{
	BodyRendererScale = FVector2D(64, 64);
	DropEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	DropEffectRenderer->CreateAnimation("StarFlash", "effect_023_starflash.png", 0, 11, 0.1f, true);
	DropEffectRenderer->SetComponentLocation({ 0, 0 });
	DropEffectRenderer->SetComponentScale({0, 0});
	DropEffectRenderer->SetOrder(ERenderOrder::Item_Back);
	DropEffectRenderer->ChangeAnimation("StarFlash");


	//DebugOn();
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	CollisionSetting();

	AActor* MainPlayer = GetWorld()->GetPawn();
	Player = dynamic_cast<APlayer*>(MainPlayer);

	DropLocation = GetActorLocation();
}

void AItem::Tick(float _DeltaTime)
{
	// �÷��̾�� �ٸ� ���̸� ����
	ARoom* PlayerCurRoom = ARoom::GetCurRoom();
	if (PlayerCurRoom != ParentRoom)
	{
		return;
	}
	// ������ �Ͻ�������� ��� ����
	if (true == APlayGameMode::IsGamePaused())
	{
		return;
	}

	Super::Tick(_DeltaTime);


	ReverseForce(_DeltaTime);

	Move(_DeltaTime);
	ClampPositionToRoom();
	//SetLocation();

	//RemoveRoomData();
	//ItemDestroy();

}

// Room�� ����� Item ������ �����Ѵ�.
void AItem::RemoveRoomData()
{
	if (false == IsDrop)
	{
		return;
	}

	ParentRoom->RemoveItem(this);
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
	if (nullptr != ImpactCollision)
	{
		ImpactCollision->SetCollisionStay(std::bind(&AItem::CollideObject, this, std::placeholders::_1));
	}
}

void AItem::Move(float _DeltaTIme)
{
	if (false == IsMove)
	{
		Direction = FVector2D::ZERO;
		return;
	}
	if (true == IsDrop)
	{
		Direction = FVector2D::ZERO;
		return;
	}

	FVector2D MoveUpperLimit = DropLocation + FVector2D(0, -10);
	FVector2D MoveLowerLimit = DropLocation + FVector2D(0, 10);
	FVector2D CurPos = GetActorLocation();

	if (CurPos.Y < MoveUpperLimit.Y )
	{
		Direction = FVector2D::DOWN;
	}
	else if (CurPos.Y > MoveLowerLimit.Y)
	{
		Direction = FVector2D::UP;
	}

	float MoveSpeed = 20.0f;
	AddActorLocation(Direction * MoveSpeed * _DeltaTIme);
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
		// �������� ���� ���ϰ� ƨ�ܳ��� ��
		FailToPickup(Player);
	}
}

void AItem::DropSuccess()
{
	IsDrop = true;
}

void AItem::FailToPickup(AActor* _Player)
{
 	FVector2D Dir = GetActorLocation() - _Player->GetActorLocation();
	Dir.Normalize(); // ���⺤��

	Force = Dir * 200.0f;
}

void AItem::ReverseForce(float _DeltaTime)
{
	if (FVector2D::ZERO == Force)
	{
		return;
	}

	FVector2D ReverseForce = -Force;
	ReverseForce.Normalize();

	Force += ReverseForce * _DeltaTime * 150.0f;
	
	if (20.0f >= Force.Length())
	{
		Force = FVector2D::ZERO;	
	}

	if (true == IsAtBoundary) // �ݻ�
	{
		FVector2D BoundaryNormal = FVector2D::ZERO;

		if (Force.X > 0) // ������ ���
		{
			BoundaryNormal = FVector2D(-1.0f, 0.0f);
		}
		else if (Force.X < 0) // ���� ���
		{
			BoundaryNormal = FVector2D(1.0f, 0.0f);
		}
		else if (Force.Y > 0) // �Ʒ� ���
		{
			BoundaryNormal = FVector2D(0.0f, 1.0f);
		}
		else // �� ���
		{
			BoundaryNormal = FVector2D(0.0f, -1.0f);
		}

		// �ݻ� ó��
		FVector2D Result =  Reflect(BoundaryNormal);
		Force *= Result.GetNormal();

		IsAtBoundary = false;
	}
	
	FVector2D Result = Force;
	AddActorLocation(Force * _DeltaTime);
}

void AItem::SetLocation()
{
	if (false == IsDrop)
	{
		return;
	}
	if (false == IsOwnedByPlayer)
	{
		return;
	}

	FVector2D PlayerPos = Player->GetActorLocation();
	SetActorLocation(PlayerPos);
}

void AItem::AreaWideAttack(AActor* _Actor)
{
	ARoom* Room = CastActorToType<ARoom>(_Actor);
	if (nullptr != Room)
	{
		return;
	}

	FVector2D Dir = _Actor->GetActorLocation() - GetActorLocation();
	Dir.Normalize(); // ���⺤��
	
	APlayer* Player = CastActorToType<APlayer>(_Actor);
	if (nullptr != Player)
	{	
		Player->ApplyDamaged(Player, 2, Dir); // ��ź�� �÷��̾�� ��Ʈ ��ĭ�� ���ظ� �ش�.
	}

	AMonster* Monster = CastActorToType<AMonster>(_Actor);
	if (nullptr != Monster)
	{
		Monster->ApplyDamaged(Monster, Att, Dir); // ��ź�� ���Ϳ��� ���ظ� ������.
	}

	ARoomObject* Object = CastActorToType<ARoomObject>(_Actor);
	if (nullptr != Object)
	{
		Object->ApplyDamaged(this);
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
	Dir.Normalize(); // ���⺤��
	Force = Dir * 200.0f;
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

void AItem::CollideObject(AActor* _Object)
{
	ARoomObject* Object = dynamic_cast<ARoomObject*>(_Object);
	if (nullptr == Object)
	{
		return;
	}
	Force = FVector2D::ZERO;
}

FVector2D AItem::Reflect(FVector2D _Dir)
{
	FVector2D ReflectedForce = Force - (_Dir * (2.0f * Force.Dot(_Dir)));
	return ReflectedForce;
}

AItem::~AItem()
{
}

