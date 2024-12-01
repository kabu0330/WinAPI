#include "PreCompile.h"
#include "Poop.h"
#include <EngineBase/EngineRandom.h>
#include "DecalObject.h"
#include "Room.h"

APoop::APoop()
{
	SetName("Poop");
	BodyRendererScale = { 64, 64 };	
	FVector2D BodyCollisionScale = { 55, 55 };
	FVector2D BlockingPathCollisionScale = { 55 , 55 };
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
	BodyCollision->SetCollisionType(ECollisionType::Rect);

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

void APoop::CreateGib()
{
	UEngineRandom Random;
	int Index = Random.RandomInt(0, 7);

	ARoomObject* Gib = ParentRoom->CreateObject<ADecalObject>(this);
	ADecalObject* Decal = dynamic_cast<ADecalObject*>(Gib);
	if (nullptr != Decal)
	{
		Decal->SetMove(this);
	}

	Sound = UEngineSound::Play("plop.wav");

	USpriteRenderer* GibRenderer = Gib->GetBodyRenderer();
	GibRenderer->CreateAnimation("Gib", "effect_poopgibs.png", Index, Index, 0.1f, false);
	GibRenderer->SetComponentScale({ 32, 32 });
	GibRenderer->SetActive(true);
	GibRenderer->SetOrder(ERenderOrder::MonsterDeathDebris);
	GibRenderer->ChangeAnimation("Gib");
}

APoop::~APoop()
{
}
