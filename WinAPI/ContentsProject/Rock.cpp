#include "PreCompile.h"
#include "Rock.h"

ARock::ARock()
{
	SetName("Rock");
	BodyRendererScale = { 64, 64 };
	FVector2D BodyCollisionScale = { 40, 40 };
	FVector2D BlockingPathCollisionScale = { 30 , 30 };
	FVector2D BlockingPathCollisionPivot = { 0, 5 };

	CanExplode = true; // Bomb�� ��ȣ �ۿ� ���� ����
	IsTearDamageable = false;
	IsBlockingPath = true;
	IsAttackable = false;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("TINTEDROCKS0", "rocks.png", 0, 0);
	BodyRenderer->CreateAnimation("TINTEDROCKS1", "rocks.png", 1, 1);
	BodyRenderer->CreateAnimation("TINTEDROCKS2", "rocks.png", 2, 2);
	BodyRenderer->CreateAnimation("TINTEDROCKS3", "rocks.png", 5, 5);
	BodyRenderer->CreateAnimation("TINTEDROCKS4", "rocks.png", 9, 9);
	BodyRenderer->CreateAnimation("TINTEDROCKS5", "rocks.png", 11, 11);
	BodyRenderer->CreateAnimation("METALBLOCKS", "rocks.png", 4, 4);
	BodyRenderer->SetComponentLocation({ 0, 0 });
	BodyRenderer->SetComponentScale(BodyRendererScale);
	BodyRenderer->SetOrder(ERenderOrder::Object_Back);
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
	
	Hp = 0;

	BodyRenderer->Destroy();
	BodyRenderer = nullptr;

	DestroyCollision();
	Destroy(0.5f);
}

ARock::~ARock()
{
}
