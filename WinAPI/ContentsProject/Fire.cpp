#include "PreCompile.h"
#include "Fire.h"

AFire::AFire()
{
	SetName("Fire");
	BodyRendererScale = { 64, 64 };
	FVector2D BodyCollisionScale = { 40, 40 };
	FVector2D BlockingPathCollisionScale = { 30 , 30 };
	FVector2D BlockingPathCollisionPivot = { 0, 5 };

	Hp = 4;
	CanExplode = false;
	IsTearDamageable = true;
	IsBlockingPath = false;
	IsAttackable = true;

	float BurningFrame = 0.2f;
	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("FireBottom_Flame", "fire_bottom.png", {0, 1, 4, 5}, BurningFrame);
	BodyRenderer->CreateAnimation("FireBottom_Burned", "fire_bottom.png", 1, 1, 0.1f, false);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale);
	BodyRenderer->SetOrder(ERenderOrder::Object_Back);
	BodyRenderer->ChangeAnimation("FireBottom_Flame");
	BodyRenderer->SetActive(true);

	//FlameBaseRenderer = CreateDefaultSubObject<USpriteRenderer>();
	//FlameBaseRenderer->CreateAnimation("FireBottom_Flame", "fire_top.png", 7, 13, BurningFrame);
	//FlameBaseRenderer->CreateAnimation("FireBottom_Burned", "fire_top.png", 6, 6, 0.1f, false);
	//FlameBaseRenderer->SetComponentLocation({ 0, -20 });
	//FlameBaseRenderer->SetComponentScale(BodyRendererScale);
	//FlameBaseRenderer->SetOrder(ERenderOrder::Object_Back);
	//FlameBaseRenderer->ChangeAnimation("FireBottom_Flame");
	//FlameBaseRenderer->SetActive(true);
	//FlameBaseRenderer->SetAlphaFloat(0.5f);

	FlameHeaderRenderer = CreateDefaultSubObject<USpriteRenderer>();
	FlameHeaderRenderer->CreateAnimation("FireBottom_Flame", "fire_top.png", 0, 5, BurningFrame);
	FlameHeaderRenderer->CreateAnimation("FireBottom_Burned", "fire_top.png", 6, 6, 0.1f, false);
	FlameHeaderRenderer->SetComponentLocation({ 0, -30 });
	FlameHeaderRenderer->SetComponentScale(BodyRendererScale);
	FlameHeaderRenderer->SetOrder(ERenderOrder::Object_Back);
	FlameHeaderRenderer->ChangeAnimation("FireBottom_Flame");
	FlameHeaderRenderer->SetActive(true);
	FlameHeaderRenderer->SetAlphaFloat(0.8f);



	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale(BodyCollisionScale);
	BodyCollision->SetCollisionGroup(ECollisionGroup::Object);
	BodyCollision->SetCollisionType(ECollisionType::Circle);

	//BlockingPathCollision = CreateDefaultSubObject<U2DCollision>();
	//BlockingPathCollision->SetComponentLocation(BlockingPathCollisionPivot);
	//BlockingPathCollision->SetComponentScale(BlockingPathCollisionScale);
	//BlockingPathCollision->SetCollisionGroup(ECollisionGroup::Object);
	//BlockingPathCollision->SetCollisionType(ECollisionType::Rect);
}

AFire::~AFire()
{
}

void AFire::BeginPlay()
{
	Super::BeginPlay();
	ARoomObject::BeginPlay();
}

void AFire::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	ARoomObject::Tick(_DeltaTime);
}

