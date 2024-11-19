#include "PreCompile.h"
#include "Item.h"

#include <EngineCore/EngineAPICore.h>
#include "Room.h"
#include "PlayGameMode.h"
#include "Player.h"

AItem::AItem()
{
	SetName("Item");
	BodyRendererScale = { 64, 64 };
	BodyCollisionScale = { 32, 32 };
	ItemCount = 1;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Heart", "pickup_001_heart.png", 0, 0);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale);
	BodyRenderer->SetOrder(ERenderOrder::Item);
	BodyRenderer->ChangeAnimation("Heart");
	BodyRenderer->SetActive(true);

	PlayerCollision = CreateDefaultSubObject<U2DCollision>();
	PlayerCollision->SetComponentLocation({ 0, 0 });
	PlayerCollision->SetComponentScale(BodyCollisionScale);
	PlayerCollision->SetCollisionGroup(ECollisionGroup::Item);
	PlayerCollision->SetCollisionType(ECollisionType::Rect);

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
}

void AItem::CollisionSetting()
{
	if (nullptr != PlayerCollision)
	{
		PlayerCollision->SetCollisionEnter(std::bind(&AItem::Drop, this, std::placeholders::_1));
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

void AItem::FailToPickup(APlayer* _Player)
{
 	FVector2D Dir = GetActorLocation() - _Player->GetActorLocation();
	Dir.Normalize(); // 방향벡터

	Force = Dir * 150.0f;

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

	AddActorLocation(Force * _DeltaTime);
}


void AItem::IdleAnimation()
{
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	TimeElapsed += DeltaTime;



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
