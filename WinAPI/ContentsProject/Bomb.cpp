#include "PreCompile.h"
#include "Bomb.h"

#include <EngineCore/EngineAPICore.h>
#include "Room.h"
#include "PlayGameMode.h"
#include "Player.h"
#include "DecalObject.h"

ABomb::ABomb()
{
	SetName("Bomb");
	BodyRendererScale = {192, 192};
	BodyCollisionScale = { 48, 48 };
	ItemCount = 1;
	Att = 30; // 몬스터에게 가할 피해
	FVector2D UniversalScale = { 150, 150 };
	ItemType = EItemType::USE;

	DropRenderer = CreateDefaultSubObject<USpriteRenderer>();
	DropRenderer->CreateAnimation("Bomb", "bomb.png", 8, 8, 0.3f, false);
	DropRenderer->CreateAnimation("DropEffect", "Drop.png", 0, 3, 0.15f, false);
	DropRenderer->SetComponentLocation({ 0, 0 });
	DropRenderer->SetComponentScale(BodyRendererScale);
	DropRenderer->SetOrder(ERenderOrder::Item);
	DropRenderer->ChangeAnimation("Bomb");
	DropRenderer->SetActive(true);

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Bomb", "bomb.png", 8, 8, 0.3f, false);
	BodyRenderer->CreateAnimation("BombTimer2.0", "Bomb", { 0, 1, 2, 1, 0, 3 }, 0.2f);
	BodyRenderer->CreateAnimation("BombTimer1.0", "Bomb", { 0, 1, 2, 1, 0, 3 }, 0.05f);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale);
	BodyRenderer->SetOrder(ERenderOrder::Item_ObjectFront);
	BodyRenderer->ChangeAnimation("Bomb");
	BodyRenderer->SetActive(false);

	BobmSparkEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BobmSparkEffectRenderer->CreateAnimation("Spark", "bomb_spark.png", 0, 7, 0.2f);
	BobmSparkEffectRenderer->SetComponentLocation({ -16, -16 });
	BobmSparkEffectRenderer->SetComponentScale(BodyRendererScale * 0.3f);
	BobmSparkEffectRenderer->SetOrder(ERenderOrder::Item_ObjectFront);
	BobmSparkEffectRenderer->ChangeAnimation("Spark");
	BobmSparkEffectRenderer->SetActive(false);

	ExplosionEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	ExplosionEffectRenderer->CreateAnimation("Explosion", "Explosion.png", 0, 11, 0.12f, false);
	ExplosionEffectRenderer->SetComponentLocation({ 0, 50 });
	ExplosionEffectRenderer->SetComponentScale(UniversalScale * 4);
	ExplosionEffectRenderer->SetOrder(ERenderOrder::ItemEffect);
	ExplosionEffectRenderer->ChangeAnimation("Explosion");
	ExplosionEffectRenderer->SetActive(false);

	PlayerCollision = CreateDefaultSubObject<U2DCollision>();
	PlayerCollision->SetComponentLocation({ 0, 0 });
	PlayerCollision->SetComponentScale(BodyCollisionScale);
	PlayerCollision->SetCollisionGroup(ECollisionGroup::Item);
	PlayerCollision->SetCollisionType(ECollisionType::Circle);

	// 폭발 충돌체
	ImpactCollision = CreateDefaultSubObject<U2DCollision>();
	ImpactCollision->SetComponentLocation({ 0, 0 });
	ImpactCollision->SetComponentScale(BodyCollisionScale);
	ImpactCollision->SetCollisionGroup(ECollisionGroup::Item_Impact);
	ImpactCollision->SetCollisionType(ECollisionType::Rect);
	ImpactCollision->SetActive(false);

	UniversalCollision = CreateDefaultSubObject<U2DCollision>();
	UniversalCollision->SetComponentLocation({ 0, 0 });
	UniversalCollision->SetComponentScale(UniversalScale);
	UniversalCollision->SetCollisionGroup(ECollisionGroup::Item_UniversalHit);
	UniversalCollision->SetCollisionType(ECollisionType::Rect);
	UniversalCollision->SetActive(false);
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

bool ABomb::EatFunction(APlayer* _Player)
{
	int CurItemCount = _Player->GetItemCount(GetName());
	if (CurItemCount > 99)
	{
		return false; // 못먹으면 튕겨낸다.
	}

	Sound = UEngineSound::Play("lift.wav");
	Sound.SetVolume(0.5f);

	IsPickedUp = true;
	DropRenderer->ChangeAnimation("DropEffect");
	TimeEventer.PushEvent(0.8f, [this]() {DropRenderer->SetActive(false); });

	return true;
}

void ABomb::UseItem(APlayer* _Player)
{
	Sound = UEngineSound::Play("pop_impact_09.wav");

	if (nullptr !=_Player)
	{
		FVector2D Pos = _Player->GetActorLocation();
		SetActorLocation({ Pos.X, Pos.Y - 5.0f });
	}

	BodyRenderer->SetActive(true);
	BobmSparkEffectRenderer->SetActive(true); // 폭탄에 불붙는 애니메이션 출력
	ImpactCollision->SetActive(true); // 눈물 공격으로 밀어내는 상호작용 출력

	TimeEventer.PushEvent(0.8f, [this]() {
		BodyRenderer->ChangeAnimation("BombTimer2.0");
		BodyRenderer->SetComponentScale(BodyRendererScale * 0.25);
		BobmSparkEffectRenderer->SetComponentLocation({ -12, -14 }); });

	TimeEventer.PushEvent(2.0f, [this]() {
		BodyRenderer->ChangeAnimation("BombTimer1.0");
		BodyRenderer->SetComponentScale(BodyRendererScale * 0.25); });

	TimeEventer.PushEvent(3.0f, std::bind(&ABomb::Explosion, this));
}

void ABomb::Explosion()
{
	Sound = UEngineSound::Play("explosion_weak1.wav");

	ImpactCollision->SetActive(false);

	UniversalCollision->SetActive(true);
	UniversalCollision->Destroy(0.1f);

	ExplosionEffectRenderer->SetActive(true);

	BodyRenderer->SetActive(false);
	BobmSparkEffectRenderer->SetActive(false);

	ARoomObject* Bombradius = ParentRoom->CreateObject<ADecalObject>(this, {0, 15});
	USpriteRenderer* BombradiusRenderer = Bombradius->GetBodyRenderer();
	BombradiusRenderer->CreateAnimation("Bombbradius", "effect_017_bombradius.png", 0, 1, 0.05f, false);
	BombradiusRenderer->SetComponentScale({ 216, 160 });
	BombradiusRenderer->SetActive(true);
	BombradiusRenderer->SetAlphaFloat(0.6f);
	BombradiusRenderer->SetOrder(ERenderOrder::Decal);
	BombradiusRenderer->ChangeAnimation("Bombbradius");
	Destroy(2.5f);
}

ABomb::~ABomb()
{
}
