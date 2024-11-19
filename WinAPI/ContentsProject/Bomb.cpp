#include "PreCompile.h"
#include "Bomb.h"

#include <EngineCore/EngineAPICore.h>
#include "Room.h"
#include "PlayGameMode.h"
#include "Player.h"

ABomb::ABomb()
{
	SetName("Bomb");
	BodyRendererScale = {192, 192};
	BodyCollisionScale = { 32, 32 };
	ItemCount = 1;
	Att = 60; // 몬스터에게 가할 피해

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Bomb", "bomb.png", 8, 8, 0.3f, false);
	BodyRenderer->CreateAnimation("HalfHeart", "HalfHeart", 0, 3, 0.3f);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale);
	BodyRenderer->SetOrder(ERenderOrder::Item);
	BodyRenderer->ChangeAnimation("Bomb");
	BodyRenderer->SetActive(true);

	BobmSparkEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BobmSparkEffectRenderer->CreateAnimation("Bomb", "bomb.png", 8, 8, 0.3f, false);
	BobmSparkEffectRenderer->CreateAnimation("HalfHeart", "HalfHeart", 0, 3, 0.3f);
	BobmSparkEffectRenderer->SetComponentLocation({ 0, 0 });
	BobmSparkEffectRenderer->SetComponentScale(BodyRendererScale);
	BobmSparkEffectRenderer->SetOrder(ERenderOrder::Item);
	BobmSparkEffectRenderer->ChangeAnimation("Bomb");
	BobmSparkEffectRenderer->SetActive(true);

	FlamesEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	FlamesEffectRenderer->CreateAnimation("Bomb", "bomb.png", 8, 8, 0.3f, false);
	FlamesEffectRenderer->CreateAnimation("HalfHeart", "HalfHeart", 0, 3, 0.3f);
	FlamesEffectRenderer->SetComponentLocation({ 0, 0 });
	FlamesEffectRenderer->SetComponentScale(BodyRendererScale);
	FlamesEffectRenderer->SetOrder(ERenderOrder::Item);
	FlamesEffectRenderer->ChangeAnimation("Bomb");
	FlamesEffectRenderer->SetActive(true);

	ExplosionEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	ExplosionEffectRenderer->CreateAnimation("Bomb", "bomb.png", 8, 8, 0.3f, false);
	ExplosionEffectRenderer->CreateAnimation("HalfHeart", "HalfHeart", 0, 3, 0.3f);
	ExplosionEffectRenderer->SetComponentLocation({ 0, 0 });
	ExplosionEffectRenderer->SetComponentScale(BodyRendererScale);
	ExplosionEffectRenderer->SetOrder(ERenderOrder::Item);
	ExplosionEffectRenderer->ChangeAnimation("Bomb");
	ExplosionEffectRenderer->SetActive(true);


	ImpactCollision = CreateDefaultSubObject<U2DCollision>();
	ImpactCollision->SetComponentLocation({ 0, 0 });
	ImpactCollision->SetComponentScale(BodyCollisionScale);
	ImpactCollision->SetCollisionGroup(ECollisionGroup::Item_Impact);
	ImpactCollision->SetCollisionType(ECollisionType::Circle);

	UniversalCollision = CreateDefaultSubObject<U2DCollision>();
	UniversalCollision->SetComponentLocation({ 0, 0 });
	UniversalCollision->SetComponentScale({150, 150});
	UniversalCollision->SetCollisionGroup(ECollisionGroup::Item);
	UniversalCollision->SetCollisionType(ECollisionType::Rect);
	UniversalCollision->SetActive(false);
}

void ABomb::BeginPlay()
{
	Super::BeginPlay();
	AItem::BeginPlay();
	BombCollisionSetting();
	TimeEventer.PushEvent(3.0f, std::bind(&ABomb::Explosion, this));
}

void ABomb::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	AItem::Tick(_DeltaTime);
}

void ABomb::BombCollisionSetting()
{
	UniversalCollision->SetCollisionEnter(std::bind(&AItem::AreaWideAttack, this, std::placeholders::_1));
}

void ABomb::Explosion()
{
	UniversalCollision->SetActive(true);
	UniversalCollision->Destroy(0.1f);

	ExplosionEffectRenderer->SetActive(true);

	BodyRenderer->SetActive(false);
	BobmSparkEffectRenderer->SetActive(false);
	FlamesEffectRenderer->SetActive(false);

	Destroy(1.0f);
}

ABomb::~ABomb()
{
}
