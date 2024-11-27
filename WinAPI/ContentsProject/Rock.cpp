#include "PreCompile.h"
#include "Rock.h"
#include "DecalObject.h"
#include "Room.h"

ARock::ARock()
{
	SetName("Rock");
	BodyRendererScale = { 64, 64 };
	FVector2D BodyCollisionScale = { 50, 50 };
	FVector2D BlockingPathCollisionScale = { 50 , 50 };
	FVector2D BlockingPathCollisionPivot = { 0, 0 };

	CanExplode = true; // Bomb과 상호 작용 가능 조건
	IsTearDamageable = false;
	IsBlockingPath = true;
	IsAttackable = false;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("TINTEDROCKS0", "rocks.png", 0, 0  , 0.1f, false);
	BodyRenderer->CreateAnimation("TINTEDROCKS1", "rocks.png", 1, 1  , 0.1f, false);
	BodyRenderer->CreateAnimation("TINTEDROCKS2", "rocks.png", 2, 2  , 0.1f, false);
	BodyRenderer->CreateAnimation("TINTEDROCKS3", "rocks.png", 5, 5  , 0.1f, false);
	BodyRenderer->CreateAnimation("TINTEDROCKS4", "rocks.png", 9, 9  , 0.1f, false);
	BodyRenderer->CreateAnimation("TINTEDROCKS5", "rocks.png", 11, 11, 0.1f, false);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale);
	BodyRenderer->SetOrder(ERenderOrder::Object_PlayerBack);
	BodyRenderer->ChangeAnimation("TintedRocks0");
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

void ARock::BeginPlay()
{
	Super::BeginPlay();
	ARoomObject::BeginPlay();
}

void ARock::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	ARoomObject::Tick(_DeltaTime);
}

void ARock::DestroyRenderer()
{
	if (false == CanExplode)
	{
		return;
	}
	if (nullptr == BodyRenderer)
	{
		return;
	}
	
	Sound = UEngineSound::Play("explosion_debris1.wav");

	Hp = 0;

	BodyRenderer->Destroy();
	BodyRenderer = nullptr;

	ARoomObject* Debris = ParentRoom->CreateObject<ADecalObject>(this);
	ADecalObject* Decal = dynamic_cast<ADecalObject*>(Debris);
	if (nullptr != Decal)
	{
		USpriteRenderer* GibRenderer = Debris->GetBodyRenderer();
		GibRenderer->CreateAnimation("Decal", "rocks.png", 3, 3, 0.1f, false);
		GibRenderer->SetComponentScale(BodyRendererScale);
		GibRenderer->SetActive(true);
		GibRenderer->SetOrder(ERenderOrder::MonsterDeathDebris);
		GibRenderer->ChangeAnimation("Decal");
	}

	DestroyCollision();
	Destroy(0.5f);
}

void ARock::DestroyCollision()
{
	if (false == IsDeath())
	{
		return;
	}
	if (nullptr == BodyCollision)
	{
		return;
	}

	IsBlockingPath = false;

	BodyCollision->Destroy();
	BodyCollision = nullptr;
	//SpawnItem();

	if (nullptr != BlockingPathCollision)
	{
		BlockingPathCollision->Destroy();
		BlockingPathCollision = nullptr;
	}
}

ARock::~ARock()
{
}
