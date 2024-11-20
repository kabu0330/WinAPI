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
	FVector2D UnversalScale = { 150, 150 };


	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Bomb", "bomb.png", 8, 8, 0.3f, false);
	BodyRenderer->CreateAnimation("BombTimer2.0", "Bomb", { 0, 1, 2, 1, 0, 3 }, 0.2f);
	BodyRenderer->CreateAnimation("BombTimer1.0", "Bomb", { 0, 1, 2, 1, 0, 3 }, 0.05f);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale);
	BodyRenderer->SetOrder(ERenderOrder::Item);
	BodyRenderer->ChangeAnimation("Bomb");
	BodyRenderer->SetActive(true);

	BobmSparkEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BobmSparkEffectRenderer->CreateAnimation("Spark", "bomb_spark.png", 0, 7, 0.2f);
	BobmSparkEffectRenderer->SetComponentLocation({ -16, -16 });
	BobmSparkEffectRenderer->SetComponentScale(BodyRendererScale * 0.3f);
	BobmSparkEffectRenderer->SetOrder(ERenderOrder::Item_Front);
	BobmSparkEffectRenderer->ChangeAnimation("Spark");
	BobmSparkEffectRenderer->SetActive(true);

	ExplosionEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	ExplosionEffectRenderer->CreateAnimation("Explosion", "Explosion.png", 0, 11, 0.1f, false);
	ExplosionEffectRenderer->SetComponentLocation({ 0, 50 });
	ExplosionEffectRenderer->SetComponentScale(UnversalScale * 4);
	ExplosionEffectRenderer->SetOrder(ERenderOrder::ItemEffect);
	ExplosionEffectRenderer->ChangeAnimation("Explosion");
	ExplosionEffectRenderer->SetActive(false);


	ImpactCollision = CreateDefaultSubObject<U2DCollision>();
	ImpactCollision->SetComponentLocation({ 0, 0 });
	ImpactCollision->SetComponentScale(BodyCollisionScale);
	ImpactCollision->SetCollisionGroup(ECollisionGroup::Item_Impact);
	ImpactCollision->SetCollisionType(ECollisionType::Circle);

	UniversalCollision = CreateDefaultSubObject<U2DCollision>();
	UniversalCollision->SetComponentLocation({ 0, 0 });
	UniversalCollision->SetComponentScale(UnversalScale);
	UniversalCollision->SetCollisionGroup(ECollisionGroup::Item_UniversalHit);
	UniversalCollision->SetCollisionType(ECollisionType::Rect);
	UniversalCollision->SetActive(false);
}

void ABomb::BeginPlay()
{
	Super::BeginPlay();
	AItem::BeginPlay();
	BombCollisionSetting();

	TimeEventer.PushEvent(0.8f, [this]() {
		BodyRenderer->ChangeAnimation("BombTimer2.0"); 
		BodyRenderer->SetComponentScale(BodyRendererScale * 0.25); 
		BobmSparkEffectRenderer->SetComponentLocation({ -12, -14 }); });

	TimeEventer.PushEvent(2.5f, [this]() {
		BodyRenderer->ChangeAnimation("BombTimer1.0"); 
		BodyRenderer->SetComponentScale(BodyRendererScale * 0.25); });

	TimeEventer.PushEvent(4.0f, std::bind(&ABomb::Explosion, this));
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

	Destroy(1.5f);
}

ABomb::~ABomb()
{
}
