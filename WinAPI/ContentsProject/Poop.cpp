#include "PreCompile.h"
#include "Poop.h"

APoop::APoop()
{
	SetName("Poop");
	BodyRendererScale = { 64, 64 };	
	FVector2D BodyCollisionScale = { 40, 40 };
	FVector2D BlockingPathCollisionScale = { 30 , 30 };
	FVector2D BlockingPathCollisionPivot = { 0, 5 };

	Hp = 3;
	CanExplode = true;
	IsTearDamageable = true;
	IsBlockingPath = true;
	IsAttackable = false;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("CORNY_POOP", "poops.png", 10, 10);
	BodyRenderer->CreateAnimation("CORNY_POOP1", "poops.png", 12, 12);
	BodyRenderer->CreateAnimation("CORNY_POOP2", "poops.png", 13, 13);
	BodyRenderer->CreateAnimation("CORNY_POOP3", "poops.png", 14, 14);

	BodyRenderer->CreateAnimation("GOLDEN_POOP", "poops.png", 15, 15);
	BodyRenderer->CreateAnimation("GOLDEN_POOP1", "poops.png", 17, 17);
	BodyRenderer->CreateAnimation("GOLDEN_POOP2", "poops.png", 18, 18);
	BodyRenderer->CreateAnimation("GOLDEN_POOP3", "poops.png", 19, 19);

	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale);
	BodyRenderer->SetOrder(ERenderOrder::Object_Back);
	BodyRenderer->ChangeAnimation("CORNY_POOP");
	BodyRenderer->SetActive(true);

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale(BodyCollisionScale);
	BodyCollision->SetCollisionGroup(ECollisionGroup::Object);
	BodyCollision->SetCollisionType(ECollisionType::Circle);

	BlockingPathCollision = CreateDefaultSubObject<U2DCollision>();
	BlockingPathCollision->SetComponentLocation(BlockingPathCollisionPivot);
	BlockingPathCollision->SetComponentScale(BlockingPathCollisionScale);
	BlockingPathCollision->SetCollisionGroup(ECollisionGroup::Object);
	BlockingPathCollision->SetCollisionType(ECollisionType::Rect);

}

void APoop::BeginPlay()
{
	Super::BeginPlay();
	ARoomObject::BeginPlay();

}

void APoop::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	ARoomObject::Tick(_DeltaTime);
}

APoop::~APoop()
{
}
