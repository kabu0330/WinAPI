#include "PreCompile.h"
#include "RoomObject.h"

#include <EngineBase/EngineMath.h>
#include "ContentsEnum.h"
#include "Player.h"
#include "Monster.h"

ARoomObject::ARoomObject()
{
	Scale = { 64, 64 };
	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Rock", "rocks.png", 0, 0);
	BodyRenderer->ChangeAnimation("Rock");
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(Scale);
	BodyRenderer->SetOrder(ERenderOrder::Object_Back);
	BodyRenderer->SetActive(true);

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ Scale.iX() - 15, Scale.iY() - 15});
	BodyCollision->SetCollisionGroup(ECollisionGroup::Object);
	BodyCollision->SetCollisionType(ECollisionType::Rect);

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