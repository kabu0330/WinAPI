#include "PreCompile.h"
#include "RoomObject.h"

ARoomObject::ARoomObject()
{
	Scale = { 64, 64 };
	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("CORNY_POOP0", "poops.png", 10, 10);
	BodyRenderer->CreateAnimation("CORNY_POOP1", "poops.png", 11, 11);
	BodyRenderer->CreateAnimation("CORNY_POOP2", "poops.png", 12, 12);
	BodyRenderer->CreateAnimation("CORNY_POOP3", "poops.png", 13, 13);
	BodyRenderer->CreateAnimation("CORNY_POOP4", "poops.png", 14, 14);

	BodyRenderer->CreateAnimation("GOLDEN_POOP0", "poops.png", 15, 15);
	BodyRenderer->CreateAnimation("GOLDEN_POOP1", "poops.png", 16, 16);
	BodyRenderer->CreateAnimation("GOLDEN_POOP2", "poops.png", 17, 17);
	BodyRenderer->CreateAnimation("GOLDEN_POOP3", "poops.png", 18, 18);
	BodyRenderer->CreateAnimation("GOLDEN_POOP4", "poops.png", 19, 19);

	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(Scale);
	BodyRenderer->SetOrder(ERenderOrder::Object_Back);
	BodyRenderer->ChangeAnimation("CORNY_POOP0");
	BodyRenderer->SetActive(true);

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ Scale.iX() - 15, Scale.iY() - 15 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Object);
	BodyCollision->SetCollisionType(ECollisionType::Circle);

	Hp = 4;
	IsTearDamageable = true;
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
	BodyCollision->SetCollisionStay(std::bind(&ARoomObject::Blocker, this, std::placeholders::_1));
}

void ARoomObject::Blocker(AActor* _Actor)
{
	// 플레이어나 몬스터가 지나갈 수 없도록하는 로직
	if (_Actor == nullptr)
	{
		return;
	}

	// Actor의 이동을 차단하는 로직
	FVector2D ActorPos = _Actor->GetActorLocation();
	APlayer * Player = dynamic_cast<APlayer*>(_Actor);
	if (nullptr != Player)
	{
		PlayerCollision(Player, ActorPos);
		return;
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Actor); // 몬스터라면
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

	switch (Hp)
	{
	case 4:
		BodyRenderer->ChangeAnimation("CORNY_POOP0");
		break;
	case 3:
		BodyRenderer->ChangeAnimation("CORNY_POOP1");
		break;
	case 2:
		BodyRenderer->ChangeAnimation("CORNY_POOP2");
		break;
	case 1:
		BodyRenderer->ChangeAnimation("CORNY_POOP3");
		break;
	case 0: // 충돌체가 파괴되고 남은 잔해물
		BodyRenderer->ChangeAnimation("CORNY_POOP4");
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