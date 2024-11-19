#include "PreCompile.h"
#include "Bomb.h"

#include <EngineCore/EngineAPICore.h>
#include "Room.h"
#include "PlayGameMode.h"
#include "Player.h"

ABomb::ABomb()
{
	SetName("Bomb");
	BodyRendererScale = { 64, 64 };
	BodyCollisionScale = { 32, 32 };
	ItemCount = 1;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Heart", "Heart", 0, 3, 0.3f);
	BodyRenderer->CreateAnimation("HalfHeart", "HalfHeart", 0, 3, 0.3f);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale);
	BodyRenderer->SetOrder(ERenderOrder::Item);
	BodyRenderer->ChangeAnimation("Heart");
	BodyRenderer->SetActive(true);

	UniversalCollision = CreateDefaultSubObject<U2DCollision>();
	UniversalCollision->SetComponentLocation({ 0, 0 });
	UniversalCollision->SetComponentScale(BodyCollisionScale);
	UniversalCollision->SetCollisionGroup(ECollisionGroup::Item);
	UniversalCollision->SetCollisionType(ECollisionType::Rect);
}

void ABomb::BeginPlay()
{
	Super::BeginPlay();
	AItem::BeginPlay();
}

void ABomb::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AItem::Tick(_DeltaTime);
}

ABomb::~ABomb()
{
}
