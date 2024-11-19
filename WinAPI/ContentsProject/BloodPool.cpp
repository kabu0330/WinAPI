#include "PreCompile.h"
#include "BloodPool.h"

ABloodPool::ABloodPool()
{
	SetName("BloodPool");
	BodyRendererScale = { 64, 64 };

	CanExplode = false;
	IsTearDamageable = false;
	IsBlockingPath = false;
	IsAttackable = false;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("bloodpool00", "effect_bloodpool.png",  0,  0, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool01", "effect_bloodpool.png",  1,  1, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool02", "effect_bloodpool.png",  2,  2, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool03", "effect_bloodpool.png",  3,  3, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool04", "effect_bloodpool.png",  4,  4, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool05", "effect_bloodpool.png",  5,  5, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool06", "effect_bloodpool.png",  6,  6, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool07", "effect_bloodpool.png",  7,  7, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool08", "effect_bloodpool.png",  8,  8, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool09", "effect_bloodpool.png",  9,  9, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool10", "effect_bloodpool.png", 10, 10, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool11", "effect_bloodpool.png", 11, 11, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool12", "effect_bloodpool.png", 12, 12, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool13", "effect_bloodpool.png", 13, 13, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool14", "effect_bloodpool.png", 14, 14, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool15", "effect_bloodpool.png", 15, 15, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool16", "effect_bloodpool.png", 16, 16, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool17", "effect_bloodpool.png", 17, 17, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool18", "effect_bloodpool.png", 18, 18, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool19", "effect_bloodpool.png", 19, 19, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool20", "effect_bloodpool.png", 20, 20, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool21", "effect_bloodpool.png", 21, 21, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool22", "effect_bloodpool.png", 22, 22, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool23", "effect_bloodpool.png", 23, 23, 0.1f, false);
	BodyRenderer->CreateAnimation("bloodpool24", "effect_bloodpool.png", 24, 24, 0.1f, false);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale);
	BodyRenderer->SetOrder(ERenderOrder::Decal);
	BodyRenderer->ChangeAnimation("bloodpool00");
	BodyRenderer->SetActive(true);
	BodyRenderer->SetAlphaFloat(0.9f);

}

void ABloodPool::BeginPlay()
{
	Super::BeginPlay();
	ARoomObject::BeginPlay();
}

void ABloodPool::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	ARoomObject::Tick(_DeltaTime);

}

ABloodPool::~ABloodPool()
{
}
