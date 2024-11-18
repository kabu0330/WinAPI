#include "PreCompile.h"
#include "RoomObject.h"

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
	Super::Tick(_DeltaTime);

	DestroyCollision();
	SwitchAnimation();
}

void ARoomObject::CollisionSetting()
{
	if (nullptr == BodyCollision)
	{
		return;
	}
	BodyCollision->SetCollisionStay(std::bind(&ARoomObject::Blocker, this, std::placeholders::_1));
}

void ARoomObject::Blocker(AActor* _Actor)
{
	// �÷��̾ ���Ͱ� ������ �� �������ϴ� ����
	if (_Actor == nullptr)
	{
		return;
	}

	// Actor�� �̵��� �����ϴ� ����
	FVector2D ActorPos = _Actor->GetActorLocation();
	APlayer * Player = dynamic_cast<APlayer*>(_Actor);
	if (nullptr != Player)
	{
		PlayerCollision(Player, ActorPos);
		return;
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Actor); // ���Ͷ��
	if (true)
	{
		MonsterCollision(Monster, ActorPos);
		return;
	}
}

int ARoomObject::ApplyDamaged(AActor* _Actor)
{
	if (true == IsDeath())
	{
		return Hp;
	}
	if (false == IsTearDamageable)
	{
		return Hp;
	}

	Hp -= 1;

	if (Hp <= 0)
	{
		Hp = 0;
	}
	return Hp;
}

void ARoomObject::SwitchAnimation()
{
	if (100 < Hp)
	{
		return;
	}
	if ("" == ObjectName)
	{
		MSGASSERT("RoomObject �� SetSprite �Լ��� �����ؾ߸� �ϴ� ������Ʈ�� �����߽��ϴ�.");
		return;
	}

	switch (Hp)
	{
	case 4:
		BodyRenderer->ChangeAnimation(ObjectName + std::to_string(0));
		break;
	case 3:
		BodyRenderer->ChangeAnimation(ObjectName + std::to_string(1));
		break;
	case 2:
		BodyRenderer->ChangeAnimation(ObjectName + std::to_string(2));
		break;
	case 1:
		BodyRenderer->ChangeAnimation(ObjectName + std::to_string(3));
		break;
	case 0: // �浹ü�� �ı��ǰ� ���� ���ع�
		BodyRenderer->ChangeAnimation(ObjectName + std::to_string(4));
		break;
	default:
		break;
	}
}

void ARoomObject::DestroyCollision()
{
	if (false == IsDeath())
	{
		return;
	}
	if (nullptr == BodyCollision)
	{
		return;
	}

	BodyCollision->Destroy();
	BodyCollision = nullptr;
}

bool ARoomObject::IsDeath()
{
	if (0 >= Hp)
	{
		IsDead = true;
		return true;
	}
	return false;
}

void ARoomObject::PlayerCollision(APlayer* _Player, FVector2D _Pos)
{
	FVector2D ActorPos = _Pos;
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
		
	if (LeftEdge > ActorLeftPos)
	{
		_Player->SetActorLocation(_Player->GetActorLocation() + FVector2D{ -1, 0 });
	}
	if (RightEdge < ActorRightPos)
	{
		_Player->SetActorLocation(_Player->GetActorLocation() + FVector2D{ 1, 0 });
	}
	if (TopEdge > ActorTopPos)
	{
		_Player->SetActorLocation(_Player->GetActorLocation() + FVector2D{ 0, -1 });
	}
	if (BotEdge < ActorBotPos)
	{
		_Player->SetActorLocation(_Player->GetActorLocation() + FVector2D{ 0, 1 });
	}
}

void ARoomObject::MonsterCollision(AMonster* _Monster, FVector2D _Pos)
{
	FVector2D ActorPos = _Pos;
	float ActorLeftPos = _Pos.X - _Monster->GetBodyCollision()->GetComponentScale().Half().X;
	float ActorRightPos = _Pos.X + _Monster->GetBodyCollision()->GetComponentScale().Half().X;
	float ActorTopPos = _Pos.Y - _Monster->GetBodyCollision()->GetComponentScale().Half().Y;
	float ActorBotPos = _Pos.Y + _Monster->GetBodyCollision()->GetComponentScale().Half().Y;

	FVector2D ThisPos = GetActorLocation();
	FVector2D ThisScale = BodyCollision->GetComponentScale().Half();

	float LeftEdge = ThisPos.X - ThisScale.X;
	float RightEdge = ThisPos.X + ThisScale.X;
	float TopEdge = ThisPos.Y - ThisScale.Y;
	float BotEdge = ThisPos.Y + ThisScale.Y;

	if (LeftEdge > ActorLeftPos)
	{
		_Monster->SetActorLocation(_Monster->GetActorLocation() + FVector2D{ -1, 0 });
	}
	if (RightEdge < ActorRightPos)
	{
		_Monster->SetActorLocation(_Monster->GetActorLocation() + FVector2D{ 1, 0 });
	}
	if (TopEdge > ActorTopPos)
	{
		_Monster->SetActorLocation(_Monster->GetActorLocation() + FVector2D{ 0, -1 });
	}
	if (BotEdge < ActorBotPos)
	{
		_Monster->SetActorLocation(_Monster->GetActorLocation() + FVector2D{ 0, 1 });
	}
}

ARoomObject::~ARoomObject()
{
}