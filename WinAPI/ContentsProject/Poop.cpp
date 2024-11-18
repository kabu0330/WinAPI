#include "PreCompile.h"
#include "Poop.h"

APoop::APoop()
{
	SetName("Poop");
	Scale = { 64, 64 };	
	Hp = 4;
	IsTearDamageable = true;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("CORNY_POOP0", "poops.png", 10, 10);
	BodyRenderer->CreateAnimation("CORNY_POOP1", "poops.png", 11, 11);
	BodyRenderer->CreateAnimation("CORNY_POOP2", "poops.png", 12, 12);
	BodyRenderer->CreateAnimation("CORNY_POOP3", "poops.png", 13, 13);
	BodyRenderer->CreateAnimation("CORNY_POOP4", "poops.png", 14, 14);

	BodyRenderer->CreateAnimation("GOLDEN_POOP0", "poops.png", 15, 15);
	BodyRenderer->CreateAnimation("GOLDEN_POOP1", "poops.png", 16, 16);
	BodyRenderer->CreateAnimation("GOLDEN_POOP2", "poops.png", 17, 17);
	BodyRenderer->CreateAnimation("GOLDEN_POOP3", "poops.png", 18, 18);
	BodyRenderer->CreateAnimation("GOLDEN_POOP4", "poops.png", 19, 19);

	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(Scale);
	BodyRenderer->SetOrder(ERenderOrder::Object_Back);
	BodyRenderer->ChangeAnimation("CORNY_POOP0");
	BodyRenderer->SetActive(true);

	BodyCollision = CreateDefaultSubObject<U2DCollision>();
	BodyCollision->SetComponentLocation({ 0, 0 });
	BodyCollision->SetComponentScale({ Scale.iX() - 15, Scale.iY() - 15 });
	BodyCollision->SetCollisionGroup(ECollisionGroup::Object);
	BodyCollision->SetCollisionType(ECollisionType::Circle);

}

APoop::~APoop()
{
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


