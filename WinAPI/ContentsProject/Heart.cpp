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
	HealAmount = 1;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Heart", "Heart", 0, 3, 0.3f);
	BodyRenderer->CreateAnimation("HalfHeart", "HalfHeart", 0, 3, 0.3f);
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
