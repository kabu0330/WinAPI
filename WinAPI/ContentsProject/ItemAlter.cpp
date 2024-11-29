#include "PreCompile.h"
#include "ItemAlter.h"

AItemAlter::AItemAlter()
{
	SetName("ItemAlter");
	BodyRendererScale = { 64, 64 };
	FVector2D BodyCollisionScale = { 30, 20 };
	FVector2D BlockingPathCollisionScale = { 30 , 20 };
	FVector2D BlockingPathCollisionPivot = { 0, 0 };

	CanExplode = false; // Bomb과 상호 작용 가능 조건
	IsTearDamageable = false;
	IsBlockingPath = true;
	IsAttackable = false;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->SetSprite("Itemaltar.png");
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale);
	BodyRenderer->SetOrder(ERenderOrder::Object_PlayerBack);
	BodyRenderer->SetActive(true);

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale(BodyCollisionScale);
	BodyCollision->SetCollisionGroup(ECollisionGroup::Object);
	BodyCollision->SetCollisionType(ECollisionType::Rect);

	BlockingPathCollision = CreateDefaultSubObject<U2DCollision>();
	BlockingPathCollision->SetComponentLocation(BlockingPathCollisionPivot);
	BlockingPathCollision->SetComponentScale(BlockingPathCollisionScale);
	BlockingPathCollision->SetCollisionGroup(ECollisionGroup::Object);
	BlockingPathCollision->SetCollisionType(ECollisionType::Rect);

	InitPos = GetActorLocation();
}

AItemAlter::~AItemAlter()
{
}

void AItemAlter::BeginPlay()
{
	Super::BeginPlay();
	ARoomObject::BeginPlay();
}

void AItemAlter::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	ARoomObject::Tick(_DeltaTime);
	BounceBack(_DeltaTime);
}

void AItemAlter::BlockPlayerCollision(APlayer* _Player, FVector2D _Pos)
{
	if (false == IsBlockingPath)
	{
		return;
	}

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

	if (true == IsCollision)
	{
		return;
	}
	IsCollision = true;
	Dir = GetActorLocation() - _Player->GetActorLocation();
	Dir.Normalize();
}

void AItemAlter::BounceBack(float _DeltaTime)
{
	if (false == IsCollision)
	{
		return;
	}

	// 진동 속도와 강도 설정
	const float OscillationSpeed = 50.0f; // 진동 속도
	const FVector2D OscillationMagnitude = Dir * 50.0f; // 진동 크기

	// 사인 함수를 이용해 진동 계산
	FVector2D Oscillation = std::sin(_DeltaTime * OscillationSpeed) * OscillationMagnitude;

	// 위치 업데이트
	FVector2D NewPos = { InitPos + Oscillation };
	BodyRenderer->SetComponentLocation(NewPos);

	TimeElapsed += _DeltaTime;
	if (TimeElapsed < Duration)
	{
		return;
	}
	BodyRenderer->SetComponentLocation(InitPos);
	IsCollision = false;
	TimeElapsed = 0.0f;
}



