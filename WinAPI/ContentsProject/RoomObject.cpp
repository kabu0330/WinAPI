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

	SwitchAnimation(); // ����, ��ź ��� ��ȣ�ۿ��ؼ� �̹����� �ٲ��� �ϴ� ���
	ChangeRenderOrder(); // ���� �̹��� ũ��� �浹 ������ ���̰� �� ���, ���̳��� ����� ���� ������ �����Ͽ� �÷��̰� �ڿ� �׷������� ����
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
	// �÷��̾ ���Ͱ� ������ �� �������ϴ� ����
	if (_Actor == nullptr)
	{
		return;
	}
	if (false == IsBlockingPath) // �渷 ������Ʈ�� ��� true�ؾ���
	{
		return;
	}

	// Actor�� �̵��� �����ϴ� ����
	FVector2D ActorPos = _Actor->GetActorLocation();
	APlayer * Player = dynamic_cast<APlayer*>(_Actor);
	if (nullptr != Player)
	{
		BlockPlayerCollision(Player, ActorPos);
		return;
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Actor); // ���Ͷ��
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
	if (true == CanExplode) // �ı��� ������ ������Ʈ���
	{
		// ��Ʈ���� ���θ� �ڽ� Ŭ�������� ��������� �ҵ� Hp = 0;
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
	case 0: // �浹ü�� �ı��ǰ� ���� ���ع�
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
		// �浹 ���� ���
		FVector2D CollisionNormal(0.0f, 0.0f);

		if (ActorLeftPos < LeftEdge)  // ���� �浹
		{
			CollisionNormal.X = -1.0f;
		}
		else if (ActorRightPos > RightEdge)  // ������ �浹
		{
			CollisionNormal.X = 1.0f;
		}

		if (ActorTopPos < TopEdge)  // ���� �浹
		{
			CollisionNormal.Y = -1.0f;
		}
		else if (ActorBotPos > BotEdge)  // �Ʒ��� �浹
		{
			CollisionNormal.Y = 1.0f;
		}

		// �̵� �ӵ� ����
		float NewSpeed = UEngineMath::Min(Velocity.Length() * 0.9f, 250.0f); 
		FVector2D NewVelocity = Velocity.GetNormal() * NewSpeed;

		// �浹 �ݴ� �������� �о��
		FVector2D Offset = CollisionNormal * 2.0f; // �о�� ����
		FVector2D NewPosition = ActorPos + Offset;

		// �÷��̾� ��ġ�� �ӵ� ����
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
		// �浹 ���� ���
		FVector2D CollisionNormal(0.0f, 0.0f);

		if (ActorLeftPos < LeftEdge)  // ���� �浹
		{
			CollisionNormal.X = -1.0f;
		}
		else if (ActorRightPos > RightEdge)  // ������ �浹
		{
			CollisionNormal.X = 1.0f;
		}

		if (ActorTopPos < TopEdge)  // ���� �浹
		{
			CollisionNormal.Y = -1.0f;
		}
		else if (ActorBotPos > BotEdge)  // �Ʒ��� �浹
		{
			CollisionNormal.Y = 1.0f;
		}

		// �浹 �ݴ� �������� �о��
		FVector2D Offset = CollisionNormal * 1.0f; // �о�� ����
		FVector2D NewPosition = ActorPos + Offset;

		// ���� ��ġ�� �ӵ� ����
		_Monster->SetActorLocation(NewPosition);
		_Monster->SetDirection(Velocity);
	}
}

ARoomObject::~ARoomObject()
{
}