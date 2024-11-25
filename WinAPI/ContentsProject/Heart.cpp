#include "PreCompile.h"
#include "Heart.h"

#include <EngineCore/EngineAPICore.h>
#include "Room.h"
#include "PlayGameMode.h"
#include "Player.h"

AHeart::AHeart()
{
	SetName("Heart");
	BodyRendererScale = { 64, 64 };
	BodyCollisionScale = { 32, 32 };
	ItemCount = 2;
	ItemType = EItemType::USE;

	DropRenderer = CreateDefaultSubObject<USpriteRenderer>();
	DropRenderer->CreateAnimation("Heart", "Heart", 0, 3, 0.3f);
	DropRenderer->CreateAnimation("HalfHeart", "HalfHeart", 0, 3, 0.3f);
	DropRenderer->SetComponentLocation({ 0, 0 });
	DropRenderer->SetComponentScale(BodyRendererScale);
	DropRenderer->SetOrder(ERenderOrder::Item);
	DropRenderer->ChangeAnimation("Heart");
	DropRenderer->SetActive(true);

	PlayerCollision = CreateDefaultSubObject<U2DCollision>();
	PlayerCollision->SetComponentLocation({ 0, 0 });
	PlayerCollision->SetComponentScale(BodyCollisionScale);
	PlayerCollision->SetCollisionGroup(ECollisionGroup::Item);
	PlayerCollision->SetCollisionType(ECollisionType::Rect);
}

void AHeart::BeginPlay()
{
	Super::BeginPlay();
	AItem::BeginPlay();
}

void AHeart::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AItem::Tick(_DeltaTime);
}

AHeart::~AHeart()
{
}
